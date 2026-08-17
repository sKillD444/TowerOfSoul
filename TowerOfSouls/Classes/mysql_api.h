#ifndef __MYSQL_API_H__
#define __MYSQL_API_H__

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <windows.h>
#endif

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <sstream>
using namespace std;

struct DBResult
{
    vector<string> columns;
    vector<vector<string>> rows;
    char **fetchRow()
    {
        if (!_built)
            _build();
        if (_cursor >= (int)rows.size())
            return nullptr;
        return (char **)_ptrs[_cursor++].data();
    }

    void resetCursor()
    {
        _cursor = 0;
    }

    uint64_t numRows() const { return (uint64_t)rows.size(); }
    unsigned int numFields() const { return (unsigned int)columns.size(); }

private:
    vector<vector<const char *>> _ptrs;
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

class MySQLCli
{
public:
    static MySQLCli &getInstance()
    {
        static MySQLCli inst;
        return inst;
    }

    bool connect(const string &host, unsigned int port,
                 const string &user, const string &password,
                 const string &database = "")
    {
        _host = host;
        _port = port;
        _user = user;
        _password = password;
        _database = database;

        _exe.clear();
        const char *candidates[] = {
            "C:\\xampp\\mysql\\bin\\mysql.exe",
            "mysql.exe",
            nullptr};

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

        string out;
        if (!runSQL("SELECT 1", out))
            return false;

        _ok = true;
        return true;
    }

    void setDatabase(const string &db) { _database = db; }
    bool isConnected() const { return _ok; }
    string lastError() const { return _lastErr; }
    void close() { _ok = false; }

    bool execute(const string &sql)
    {
        string out;
        return runSQL(sql, out);
    }

    DBResult *query(const string &sql)
    {
        string out;
        if (!runSQL(sql, out))
            return nullptr;

        auto *r = new DBResult();
        istringstream iss(out);
        string line;
        bool header = true;

        while (getline(iss, line))
        {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            if (line.empty())
                continue;

            vector<string> fields;
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
                r->columns = move(fields);
                header = false;
            }
            else
            {
                for (auto &f : fields)
                    if (f == "NULL")
                        f.clear();
                r->rows.push_back(move(fields));
            }
        }
        return r;
    }

    string escape(const string &s)
    {
        string r;
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
    MySQLCli(const MySQLCli &) = delete;
    MySQLCli &operator=(const MySQLCli &) = delete;

    bool runSQL(const string &sql, string &output)
    {
        char tmp[MAX_PATH];
        GetTempPathA(MAX_PATH, tmp);
        string tmpDir(tmp);
        string sqlFile = tmpDir + "cb_q.sql";
        string outFile = tmpDir + "cb_out.txt";

        {
            ofstream f(sqlFile, ios::binary);
            if (!f)
            {
                _lastErr = "Cannot create temp SQL file";
                return false;
            }
            f << sql;
        }

        string mysqlArgs = "\"" + _exe + "\"";
        mysqlArgs += " -u " + _user;
        mysqlArgs += " -h " + _host;
        mysqlArgs += " --port=" + to_string(_port);
        if (!_password.empty())
            mysqlArgs += " -p\"" + _password + "\"";
        if (!_database.empty())
            mysqlArgs += " \"" + _database + "\"";
        mysqlArgs += " --batch --raw --default-character-set=utf8mb4";

        string fullCmd = "cmd.exe /c \"" + mysqlArgs +
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
            _lastErr = "Failed to execute mysql.exe (err=" + to_string(GetLastError()) + ")";
            remove(sqlFile.c_str());
            return false;
        }

        WaitForSingleObject(pi.hProcess, 120000);

        DWORD exitCode = 1;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        output.clear();
        {
            ifstream fin(outFile, ios::binary);
            if (fin)
            {
                ostringstream oss;
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

    string _exe;
    string _host, _user, _password, _database;
    unsigned int _port = 3306;
    bool _ok = false;
    string _lastErr;
};

#endif // __MYSQL_API_H__
