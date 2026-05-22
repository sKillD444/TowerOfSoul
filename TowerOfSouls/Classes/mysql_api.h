#ifndef __MYSQL_API_H__
#define __MYSQL_API_H__

/**
 * mysql_api.h - MySQL CLI Wrapper for Card Battle Game
 *
 * Executes SQL via mysql.exe (XAMPP) using _popen + temp files.
 * This approach avoids the 32/64-bit DLL architecture mismatch
 * (game is 32-bit x86, XAMPP's libmariadb.dll is 64-bit x64).
 */

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <sstream>

 // ============================================================
 // DBResult - Parsed result set from SELECT queries
 // ============================================================
struct DBResult
{
    std::vector<std::string> columns;
    std::vector<std::vector<std::string>> rows;

    /// Fetch next row as char** array. Returns nullptr when exhausted.
    /// Pointers remain valid for the lifetime of this DBResult.
    char** fetchRow()
    {
        if (!_built)
            _build();
        if (_cursor >= (int)rows.size())
            return nullptr;
        return (char**)_ptrs[_cursor++].data();
    }

    void resetCursor()
    {
        _cursor = 0;
    }

    uint64_t numRows() const { return (uint64_t)rows.size(); }
    unsigned int numFields() const { return (unsigned int)columns.size(); }

private:
    std::vector<std::vector<const char*>> _ptrs;
    int _cursor = 0;
    bool _built = false;

    void _build()
    {
        _ptrs.resize(rows.size());
        for (size_t i = 0; i < rows.size(); i++)
        {
            _ptrs[i].resize(rows[i].size());
            for (size_t j = 0; j < rows[i].size(); j++)
                _ptrs[i][j] = rows[i][j].c_str();
        }
        _built = true;
        _cursor = 0;
    }
};

// ============================================================
// MySQLCli - Singleton CLI wrapper for mysql.exe
// ============================================================
class MySQLCli
{
public:
    static MySQLCli& getInstance()
    {
        static MySQLCli inst;
        return inst;
    }

    /// Connect to MySQL server (finds mysql.exe, verifies connectivity).
    /// Database can be empty; use setDatabase() later after CREATE DATABASE.
    bool connect(const std::string& host, unsigned int port,
        const std::string& user, const std::string& password,
        const std::string& database = "")
    {
        _host = host;
        _port = port;
        _user = user;
        _password = password;
        _database = database;

        // Locate mysql.exe
        _exe.clear();
        const char* candidates[] = {
            "C:\\xampp\\mysql\\bin\\mysql.exe",
            "mysql.exe",
            nullptr };

        for (int i = 0; candidates[i]; i++)
        {
            if (GetFileAttributesA(candidates[i]) != INVALID_FILE_ATTRIBUTES)
            {
                _exe = candidates[i];
                break;
            }
        }

        if (_exe.empty())
        {
            _lastErr = "mysql.exe not found";
            return false;
        }

        // Verify connectivity
        std::string out;
        if (!runSQL("SELECT 1", out))
            return false;

        _ok = true;
        return true;
    }

    void setDatabase(const std::string& db) { _database = db; }
    bool isConnected() const { return _ok; }
    std::string lastError() const { return _lastErr; }
    void close() { _ok = false; }

    /// Execute non-SELECT SQL (INSERT, UPDATE, DELETE, CREATE, etc.)
    /// Supports multiple statements separated by semicolons.
    bool execute(const std::string& sql)
    {
        std::string out;
        return runSQL(sql, out);
    }

    /// Execute SELECT query; returns new DBResult (caller must delete).
    /// Returns nullptr on error.
    DBResult* query(const std::string& sql)
    {
        std::string out;
        if (!runSQL(sql, out))
            return nullptr;

        auto* r = new DBResult();
        std::istringstream iss(out);
        std::string line;
        bool header = true;

        while (std::getline(iss, line))
        {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            if (line.empty())
                continue;

            // Parse tab-separated fields
            std::vector<std::string> fields;
            size_t start = 0;
            for (size_t pos = 0; pos <= line.size(); pos++)
            {
                if (pos == line.size() || line[pos] == '\t')
                {
                    fields.push_back(line.substr(start, pos - start));
                    start = pos + 1;
                }
            }

            if (header)
            {
                r->columns = std::move(fields);
                header = false;
            }
            else
            {
                // Convert "NULL" literal to empty string for C API compat
                for (auto& f : fields)
                    if (f == "NULL")
                        f.clear();
                r->rows.push_back(std::move(fields));
            }
        }
        return r;
    }

    /// SQL-safe string escaping (backslash-based)
    std::string escape(const std::string& s)
    {
        std::string r;
        r.reserve(s.size() + 16);
        for (char c : s)
        {
            switch (c)
            {
            case '\'':
                r += "\\'";
                break;
            case '\\':
                r += "\\\\";
                break;
            case '"':
                r += "\\\"";
                break;
            case '\0':
                r += "\\0";
                break;
            case '\n':
                r += "\\n";
                break;
            case '\r':
                r += "\\r";
                break;
            case '\x1a':
                r += "\\Z";
                break;
            default:
                r += c;
            }
        }
        return r;
    }

private:
    MySQLCli() = default;
    ~MySQLCli() = default;
    MySQLCli(const MySQLCli&) = delete;
    MySQLCli& operator=(const MySQLCli&) = delete;

    /// Write SQL to temp file, execute via mysql.exe with hidden window, capture output.
    bool runSQL(const std::string& sql, std::string& output)
    {
        // Write SQL to temp file (avoids shell escaping nightmares)
        char tmp[MAX_PATH];
        GetTempPathA(MAX_PATH, tmp);
        std::string tmpDir(tmp);
        std::string sqlFile = tmpDir + "cb_q.sql";
        std::string outFile = tmpDir + "cb_out.txt";

        {
            std::ofstream f(sqlFile, std::ios::binary);
            if (!f)
            {
                _lastErr = "Cannot create temp SQL file";
                return false;
            }
            f << sql;
        }

        // Build mysql.exe command line
        std::string mysqlArgs = "\"" + _exe + "\"";
        mysqlArgs += " -u " + _user;
        mysqlArgs += " -h " + _host;
        mysqlArgs += " --port=" + std::to_string(_port);
        if (!_password.empty())
            mysqlArgs += " -p\"" + _password + "\"";
        if (!_database.empty())
            mysqlArgs += " \"" + _database + "\"";
        mysqlArgs += " --batch --raw --default-character-set=utf8mb4";

        // Use cmd.exe /c with file redirection, CREATE_NO_WINDOW prevents console flash
        // Outer quotes required for cmd.exe /c when embedded quotes exist
        std::string fullCmd = "cmd.exe /c \"" + mysqlArgs +
            " < \"" + sqlFile + "\" > \"" + outFile + "\" 2>&1\"";

        STARTUPINFOA si = {};
        PROCESS_INFORMATION pi = {};
        si.cb = sizeof(si);

        BOOL ok = CreateProcessA(
            nullptr,
            (LPSTR)fullCmd.c_str(),
            nullptr, nullptr, FALSE,
            CREATE_NO_WINDOW,
            nullptr, nullptr,
            &si, &pi);

        if (!ok)
        {
            _lastErr = "Failed to execute mysql.exe (err=" + std::to_string(GetLastError()) + ")";
            remove(sqlFile.c_str());
            return false;
        }

        // Wait up to 120 seconds (seed data batch can be large)
        WaitForSingleObject(pi.hProcess, 120000);

        DWORD exitCode = 1;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // Read output from temp file
        output.clear();
        {
            std::ifstream fin(outFile, std::ios::binary);
            if (fin)
            {
                std::ostringstream oss;
                oss << fin.rdbuf();
                output = oss.str();
            }
        }

        remove(sqlFile.c_str());
        remove(outFile.c_str());

        if (exitCode != 0)
        {
            _lastErr = output;
            while (!_lastErr.empty() &&
                (_lastErr.back() == '\n' || _lastErr.back() == '\r'))
                _lastErr.pop_back();
            return false;
        }
        return true;
    }

    std::string _exe;
    std::string _host, _user, _password, _database;
    unsigned int _port = 3306;
    bool _ok = false;
    std::string _lastErr;
};

#endif // __MYSQL_API_H__
