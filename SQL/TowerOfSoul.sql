-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Máy chủ: 127.0.0.1
-- Thời gian đã tạo: Th7 16, 2026 lúc 01:00 AM
-- Phiên bản máy phục vụ: 10.4.32-MariaDB
-- Phiên bản PHP: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Cơ sở dữ liệu: `towerofsoul`
--

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `card_definitions`
-- (Đã xóa 4 cột: evolve_at_level, evolved_name, evolve_atk_gain, evolve_hp_gain)
--

CREATE TABLE `card_definitions` (
  `id` int(11) NOT NULL,
  `card_id` varchar(50) NOT NULL,
  `name` varchar(100) NOT NULL,
  `card_type` varchar(50) NOT NULL DEFAULT 'MONSTER',
  `role` varchar(50) NOT NULL,
  `base_atk` int(11) NOT NULL,
  `base_hp` int(11) NOT NULL,
  `cost` int(11) NOT NULL,
  `base_star` int(11) NOT NULL DEFAULT 1
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Đang đổ dữ liệu cho bảng `card_definitions`
--

INSERT INTO `card_definitions` (`id`, `card_id`, `name`, `card_type`, `role`, `base_atk`, `base_hp`, `cost`, `base_star`) VALUES
(1, 'TaVu', 'Tà Vu', 'MONSTER', 'Assassin', 90, 100, 2, 1),
(2, 'HuyetDao', 'Huyết Đao', 'MONSTER', 'Assassin', 70, 140, 1, 1),
(3, 'DaoTac', 'Đạo Tặc', 'MONSTER', 'Assassin', 80, 120, 2, 1),
(4, 'YeuKiem', 'Yêu Kiếm', 'MONSTER', 'Warrior', 45, 255, 1, 1),
(5, 'DotKich', 'Đột Kích', 'MONSTER', 'Warrior', 50, 265, 2, 1),
(6, 'TienPhong', 'Tiên Phong', 'MONSTER', 'Warrior', 55, 275, 3, 1),
(7, 'DaChuy', 'Dã Chùy', 'MONSTER', 'Warrior', 60, 280, 4, 1),
(8, 'SongDao', 'Song Đao', 'MONSTER', 'Warrior', 60, 260, 3, 1),
(9, 'TieuCot', 'Tiểu Cốt', 'MONSTER', 'Warrior', 45, 250, 1, 1),
(10, 'CotChuy', 'Cốt Chùy', 'MONSTER', 'Warrior', 50, 255, 2, 1),
(11, 'CotThuong', 'Cốt Thương', 'MONSTER', 'Warrior', 55, 260, 3, 1),
(12, 'CuongChien', 'Cuồng Chiến', 'MONSTER', 'Warrior', 65, 280, 4, 1),
(13, 'BocPha', 'Bộc Phá', 'MONSTER', 'Ranged DPS', 70, 175, 3, 1),
(14, 'MaTe', 'Ma Tế', 'MONSTER', 'Ranged DPS', 60, 200, 2, 1),
(15, 'MinhHoa', 'Minh Hỏa', 'MONSTER', 'Ranged DPS', 90, 150, 4, 1),
(16, 'ThietThuan', 'Thiết Thuẫn', 'MONSTER', 'Defender', 25, 500, 3, 1),
(17, 'CuLuc', 'Cự Lực', 'MONSTER', 'Defender', 50, 450, 4, 1),
(18, 'VoUy', 'Vô Úy', 'MONSTER', 'Defender', 40, 400, 1, 1),
(19, 'PhaChuy', 'Phá Chùy', 'MONSTER', 'Defender', 45, 430, 3, 1),
(20, 'PhongNhan', 'Phong Nhẫn', 'PLAYER', 'Assassin', 70, 140, 1, 1),
(21, 'DaAnh', 'Dạ Ảnh', 'PLAYER', 'Assassin', 75, 150, 2, 1),
(22, 'AmHuong', 'Ám Hương', 'PLAYER', 'Assassin', 80, 140, 2, 1),
(23, 'HuyetTram', 'Huyết Trảm', 'PLAYER', 'Assassin', 80, 100, 1, 1),
(24, 'ThietAnh', 'Thiết Ảnh', 'PLAYER', 'Assassin', 90, 170, 4, 1),
(25, 'DaKhach', 'Dạ Khách', 'PLAYER', 'Assassin', 80, 160, 3, 1),
(26, 'LamHiep', 'Lam Hiệp', 'PLAYER', 'Warrior', 60, 270, 4, 1),
(27, 'HacGiap', 'Hắc Giáp', 'PLAYER', 'Warrior', 75, 290, 5, 1),
(28, 'HacMoc', 'Hắc Mộc', 'PLAYER', 'Warrior', 45, 250, 1, 1),
(29, 'BichNha', 'Bích Nhã', 'PLAYER', 'Warrior', 50, 275, 2, 1),
(30, 'XichCuong', 'Xích Cương', 'PLAYER', 'Warrior', 55, 265, 2, 1),
(31, 'ThietDinh', 'Thiết Đỉnh', 'PLAYER', 'Warrior', 60, 275, 4, 1),
(32, 'LinhKhoi', 'Linh Khôi', 'PLAYER', 'Warrior', 45, 250, 1, 1),
(33, 'LoiCo', 'Lôi Cơ', 'PLAYER', 'Warrior', 50, 275, 2, 1),
(34, 'DiemLa', 'Diệm La', 'PLAYER', 'Warrior', 60, 280, 4, 1),
(35, 'VoTran', 'Vô Trần', 'PLAYER', 'Warrior', 60, 265, 3, 1),
(36, 'BaLong', 'Bá Long', 'PLAYER', 'Warrior', 75, 290, 5, 1),
(37, 'NhatDao', 'Nhất Đao', 'PLAYER', 'Warrior', 60, 260, 3, 1),
(38, 'LamNguyet', 'Lam Nguyệt', 'PLAYER', 'Ranged DPS', 75, 210, 5, 1),
(39, 'HongDiep', 'Hồng Diệp', 'PLAYER', 'Ranged DPS', 110, 170, 5, 1),
(40, 'MocLan', 'Mộc Lan', 'PLAYER', 'Ranged DPS', 65, 150, 1, 1),
(41, 'LamTinh', 'Lam Tinh', 'PLAYER', 'Ranged DPS', 60, 160, 1, 1),
(42, 'HoaTuoc', 'Hỏa Tước', 'PLAYER', 'Ranged DPS', 65, 165, 2, 1),
(43, 'LamVu', 'Lâm Vũ', 'PLAYER', 'Ranged DPS', 70, 180, 3, 1),
(44, 'DaMieu', 'Dạ Miêu', 'PLAYER', 'Ranged DPS', 70, 160, 2, 1),
(45, 'CuSon', 'Cự Sơn', 'PLAYER', 'Defender', 45, 430, 3, 1),
(46, 'HoSon', 'Hộ Sơn', 'PLAYER', 'Defender', 45, 400, 2, 1),
(47, 'HungKien', 'Hùng Kiên', 'PLAYER', 'Defender', 40, 400, 1, 1),
(48, 'AnVe', 'Ẩn Vệ', 'PLAYER', 'Defender', 40, 430, 2, 1),
(49, 'ThietThanh', 'Thiết Thành', 'PLAYER', 'Defender', 50, 500, 5, 1),
(50, 'death', 'Death', 'BOSS', 'BOSS', 200, 6000, 0, 1),
(51, 'death_clone', 'Death Clone', 'BOSS', 'BOSS', 50, 2000, 0, 1),
(52, 'lich', 'Lich', 'BOSS', 'BOSS', 100, 4000, 0, 1),
(53, 'skull', 'Skull', 'BOSS', 'BOSS', 80, 3000, 0, 1),
(54, 'black_wolf', 'Black Wolf', 'BOSS', 'BOSS', 50, 1000, 0, 1),
(55, 'blue_wolf', 'Blue Wolf', 'BOSS', 'BOSS', 50, 1000, 0, 1);

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `endless_leaderboard`
--

CREATE TABLE `endless_leaderboard` (
  `user_id` int(11) NOT NULL,
  `highest_floor` int(11) NOT NULL DEFAULT 0,
  `rank_position` int(11) NOT NULL DEFAULT 0,
  `achieved_at` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Đang đổ dữ liệu cho bảng `endless_leaderboard`
--

INSERT INTO `endless_leaderboard` (`user_id`, `highest_floor`, `rank_position`, `achieved_at`) VALUES
(1, 21, 1, '2026-05-30 08:11:11'),
(2, 8, 2, '2026-05-29 17:01:00'),
(3, 45, 0, '2026-06-01 13:30:00'),
(4, 55, 0, '2026-06-02 02:45:00'),
(5, 32, 0, '2026-06-01 12:00:00'),
(6, 15, 0, '2026-05-28 16:00:00'),
(7, 28, 0, '2026-06-01 14:45:00'),
(8, 10, 0, '2026-05-29 04:00:00'),
(9, 19, 0, '2026-06-01 09:00:00'),
(10, 25, 0, '2026-06-02 01:30:00'),
(11, 40, 0, '2026-06-02 05:00:00');

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `owned_cards`
--

CREATE TABLE `owned_cards` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `card_id` varchar(50) NOT NULL,
  `star` int(11) NOT NULL DEFAULT 1,
  `level` int(11) NOT NULL DEFAULT 1,
  `exp` int(11) NOT NULL DEFAULT 0,
  `evolve_stage` int(11) NOT NULL DEFAULT 0,
  `evolve_atk_bonus` int(11) NOT NULL DEFAULT 0,
  `evolve_hp_bonus` int(11) NOT NULL DEFAULT 0,
  `slot_index` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Đang đổ dữ liệu cho bảng `owned_cards`
--

INSERT INTO `owned_cards` (`id`, `user_id`, `card_id`, `star`, `level`, `exp`, `evolve_stage`, `evolve_atk_bonus`, `evolve_hp_bonus`, `slot_index`) VALUES
(1, 1, 'LoiCo', 1, 1, 0, 0, 0, 0, 0),
(2, 1, 'LinhKhoi', 1, 5, 0, 0, 40, 160, 0),
(3, 1, 'MocLan', 1, 5, 0, 0, 20, 60, 0),
(4, 1, 'HoaTuoc', 1, 5, 0, 0, 40, 120, 0),
(5, 1, 'ThietAnh', 1, 5, 0, 0, 20, 60, 0),
(6, 1, 'LamTinh', 1, 1, 0, 0, 0, 0, 0),
(7, 1, 'HoSon', 1, 1, 0, 0, 0, 0, 0),
(8, 1, 'LamNguyet', 1, 1, 0, 0, 0, 0, 0),
(9, 1, 'DiemLa', 1, 1, 0, 0, 0, 0, 0),
(10, 1, 'LamHiep', 1, 5, 0, 0, 40, 120, 0),
(22, 1, 'BichNha', 1, 1, 0, 0, 0, 0, 0),
(23, 1, 'HoSon', 1, 1, 0, 0, 0, 0, 0),
(24, 1, 'LamTinh', 1, 1, 0, 0, 0, 0, 0),
(25, 1, 'ThietDinh', 1, 1, 0, 0, 0, 0, 0),
(26, 1, 'LamNguyet', 1, 1, 0, 0, 0, 0, 0),
(27, 1, 'ThietAnh', 1, 1, 0, 0, 0, 0, 0),
(28, 1, 'DiemLa', 1, 1, 0, 0, 0, 0, 0),
(29, 1, 'BichNha', 1, 1, 0, 0, 0, 0, 0),
(30, 1, 'DiemLa', 1, 5, 0, 0, 40, 120, 0),
(31, 1, 'CuSon', 1, 1, 0, 0, 0, 0, 0),
(32, 1, 'CuSon', 1, 1, 0, 0, 0, 0, 0),
(33, 1, 'BaLong', 1, 1, 0, 0, 0, 0, 0),
(34, 1, 'DaAnh', 1, 1, 0, 0, 0, 0, 0),
(35, 1, 'DiemLa', 1, 1, 0, 0, 0, 0, 0),
(36, 1, 'ThietThanh', 1, 1, 0, 0, 0, 0, 0),
(37, 1, 'LamHiep', 1, 1, 0, 0, 0, 0, 0),
(38, 1, 'VoTran', 1, 1, 0, 0, 0, 0, 0),
(39, 1, 'BaLong', 1, 1, 0, 0, 0, 0, 0),
(40, 1, 'LoiCo', 1, 1, 0, 0, 0, 0, 0),
(41, 1, 'PhongNhan', 1, 1, 0, 0, 0, 0, 0),
(42, 1, 'DiemLa', 1, 1, 0, 0, 0, 0, 0),
(43, 1, 'ThietAnh', 1, 1, 0, 0, 0, 0, 0),
(44, 1, 'LamTinh', 1, 1, 0, 0, 0, 0, 0),
(45, 1, 'HacMoc', 1, 1, 0, 0, 0, 0, 0),
(46, 1, 'AnVe', 1, 1, 0, 0, 0, 0, 0),
(47, 1, 'LamTinh', 1, 1, 0, 0, 0, 0, 0),
(48, 1, 'AnVe', 1, 1, 0, 0, 0, 0, 0),
(49, 1, 'LamHiep', 1, 1, 0, 0, 0, 0, 0),
(50, 1, 'LamTinh', 1, 1, 0, 0, 0, 0, 0),
(51, 1, 'NhatDao', 1, 1, 0, 0, 0, 0, 0),
(52, 1, 'HongDiep', 1, 1, 0, 0, 0, 0, 0),
(53, 1, 'DaAnh', 1, 1, 0, 0, 0, 0, 0),
(54, 1, 'HuyetTram', 1, 1, 0, 0, 0, 0, 0),
(55, 1, 'LamTinh', 1, 1, 0, 0, 0, 0, 0),
(56, 1, 'NhatDao', 1, 1, 0, 0, 0, 0, 0),
(57, 1, 'HungKien', 1, 1, 0, 0, 0, 0, 0),
(58, 1, 'XichCuong', 1, 1, 0, 0, 0, 0, 0),
(59, 1, 'NhatDao', 1, 1, 0, 0, 0, 0, 0),
(60, 1, 'DiemLa', 1, 1, 0, 0, 0, 0, 0),
(61, 1, 'BichNha', 1, 1, 0, 0, 0, 0, 0),
(62, 1, 'HuyetTram', 1, 1, 0, 0, 0, 0, 0),
(63, 1, 'CuSon', 1, 1, 0, 0, 0, 0, 0),
(64, 1, 'PhongNhan', 1, 1, 0, 0, 0, 0, 0),
(65, 1, 'HacMoc', 1, 1, 0, 0, 0, 0, 0),
(66, 1, 'ThietAnh', 1, 1, 0, 0, 0, 0, 0),
(67, 1, 'AmHuong', 1, 1, 0, 0, 0, 0, 0),
(68, 1, 'HungKien', 1, 1, 0, 0, 0, 0, 0),
(69, 1, 'LinhKhoi', 1, 1, 0, 0, 0, 0, 0),
(70, 1, 'NhatDao', 1, 1, 0, 0, 0, 0, 0),
(71, 1, 'ThietDinh', 1, 1, 0, 0, 0, 0, 0),
(72, 1, 'HacMoc', 1, 1, 0, 0, 0, 0, 0),
(73, 1, 'LamVu', 1, 1, 0, 0, 0, 0, 0),
(74, 1, 'HacMoc', 1, 1, 0, 0, 0, 0, 0);

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `stages`
-- (Đã cập nhật 12 ải chuẩn theo yêu cầu)
--

CREATE TABLE `stages` (
  `stage_number` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `enemy_bonus_hp` int(11) NOT NULL DEFAULT 0,
  `is_boss` tinyint(1) NOT NULL DEFAULT 0,
  `gold_reward` int(11) NOT NULL DEFAULT 30
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Đang đổ dữ liệu cho bảng `stages`
--

INSERT INTO `stages` (`stage_number`, `name`, `enemy_bonus_hp`, `is_boss`, `gold_reward`) VALUES
(1, 'Ải 1 - Cửa Rừng', 0, 0, 30),
(2, 'Ải 2 - Đường Mòn', 10, 0, 35),
(3, 'Ải 3 - Trại Giam', 20, 0, 40),
(4, 'Ải 4 - Lãnh Địa Sói', 50, 1, 100),
(5, 'Ải 5 - Vùng Đất Chết', 40, 0, 50),
(6, 'Ải 6 - Mộ Cổ', 50, 0, 55),
(7, 'Ải 7 - Hầm Tối', 60, 0, 60),
(8, 'Ải 8 - Điện Phù Thủy', 100, 1, 200),
(9, 'Ải 9 - Vực Sâu', 80, 0, 70),
(10, 'Ải 10 - Cổng Ngục', 90, 0, 75),
(11, 'Ải 11 - Sảnh Đen', 100, 0, 80),
(12, 'Ải 12 - Ngai Vàng Death', 200, 1, 500);

-- --------------------------------------------------------

--
-- Cấu trúc bảng mới: `stage_enemies`
-- (Lưu thông tin quái xuất hiện theo từng Turn cho 12 ải)
--

CREATE TABLE `stage_enemies` (
  `id` int(11) NOT NULL,
  `stage_number` int(11) NOT NULL,
  `turn_number` int(11) NOT NULL DEFAULT 1 COMMENT 'Lượt thứ mấy (1 hoặc 2)',
  `card_id` varchar(50) NOT NULL COMMENT 'Mã quái trong card_definitions',
  `position_slot` int(11) NOT NULL DEFAULT 1 COMMENT 'Vị trí đứng của quái'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Đang đổ dữ liệu cho bảng `stage_enemies`
--

INSERT INTO `stage_enemies` (`id`, `stage_number`, `turn_number`, `card_id`, `position_slot`) VALUES
-- Ải 1 đến 3 (1 Turn)
(1, 1, 1, 'TaVu', 1),
(2, 1, 1, 'HuyetDao', 2),
(3, 2, 1, 'DaoTac', 1),
(4, 2, 1, 'YeuKiem', 2),
(5, 3, 1, 'DotKich', 1),
(6, 3, 1, 'TienPhong', 2),
-- Ải 4 Boss (1 Turn)
(7, 4, 1, 'skull', 1),
(8, 4, 1, 'blue_wolf', 2),
(9, 4, 1, 'black_wolf', 3),
-- Ải 5 đến 7 (2 Turn)
(10, 5, 1, 'DaChuy', 1),
(11, 5, 1, 'SongDao', 2),
(12, 5, 2, 'TieuCot', 1),
(13, 5, 2, 'CotChuy', 2),
(14, 6, 1, 'CotThuong', 1),
(15, 6, 1, 'CuongChien', 2),
(16, 6, 2, 'BocPha', 1),
(17, 6, 2, 'MaTe', 2),
(18, 7, 1, 'MinhHoa', 1),
(19, 7, 1, 'ThietThuan', 2),
(20, 7, 2, 'CuLuc', 1),
(21, 7, 2, 'VoUy', 2),
-- Ải 8 Boss (1 Turn)
(22, 8, 1, 'lich', 1),
(23, 8, 1, 'death_clone', 2),
-- Ải 9 đến 11 (2 Turn)
(24, 9, 1, 'PhaChuy', 1),
(25, 9, 1, 'TaVu', 2),
(26, 9, 2, 'HuyetDao', 1),
(27, 9, 2, 'DaoTac', 2),
(28, 10, 1, 'YeuKiem', 1),
(29, 10, 1, 'DotKich', 2),
(30, 10, 2, 'TienPhong', 1),
(31, 10, 2, 'DaChuy', 2),
(32, 11, 1, 'SongDao', 1),
(33, 11, 1, 'TieuCot', 2),
(34, 11, 2, 'CotChuy', 1),
(35, 11, 2, 'CotThuong', 2),
-- Ải 12 Boss (1 Turn)
(36, 12, 1, 'death', 1);

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `username` varchar(50) NOT NULL,
  `password` varchar(100) NOT NULL,
  `gold` int(11) NOT NULL DEFAULT 0,
  `gems` int(11) NOT NULL DEFAULT 0,
  `current_stage` int(11) NOT NULL DEFAULT 1,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `last_login` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Đang đổ dữ liệu cho bảng `users`
--

INSERT INTO `users` (`id`, `username`, `password`, `gold`, `gems`, `current_stage`, `created_at`, `last_login`) VALUES
(1, 'Dang', 'pass_hashed_123', 5880, 8950, 5, '2026-04-17 00:25:07', '2026-07-13 16:57:36'),
(2, 'Player_Test', 'pass_hashed_456', 5000, 10, 5, '2026-04-17 00:25:07', '2026-04-17 00:25:07'),
(3, 'Levi', 'pass_hashed_111', 12000, 500, 8, '2026-05-01 03:00:00', '2026-06-01 13:00:00'),
(4, 'SofM', 'pass_hashed_222', 45000, 1200, 10, '2026-05-02 07:30:00', '2026-06-02 02:15:00'),
(5, 'FakerVN', 'pass_hashed_333', 8900, 150, 6, '2026-05-15 01:20:00', '2026-06-01 11:45:00'),
(6, 'Optimus', 'pass_hashed_444', 3200, 50, 4, '2026-05-20 04:10:00', '2026-05-28 15:30:00'),
(7, 'Zeros', 'pass_hashed_555', 15600, 800, 7, '2026-04-25 02:00:00', '2026-06-01 14:00:00'),
(8, 'Slayder', 'pass_hashed_666', 2100, 20, 3, '2026-05-28 09:40:00', '2026-05-29 03:10:00'),
(9, 'Bie', 'pass_hashed_777', 6700, 310, 5, '2026-05-10 06:25:00', '2026-06-01 08:20:00'),
(10, 'Kati', 'pass_hashed_888', 9400, 420, 6, '2026-05-12 12:50:00', '2026-06-02 01:05:00'),
(11, 'Kiaya', 'pass_hashed_999', 34000, 950, 9, '2026-04-18 00:15:00', '2026-06-02 04:30:00'),
(12, 'Hello', 'dang20004', 200, 10, 1, '2026-07-05 13:28:26', '2026-07-05 13:28:26'),
(13, 'DangNe', 'Dang2004', 200, 10, 1, '2026-07-06 11:33:57', '2026-07-06 11:33:57');

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `user_stage_progress`
--

CREATE TABLE `user_stage_progress` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `stage_number` int(11) NOT NULL,
  `stars_earned` int(11) NOT NULL DEFAULT 0,
  `is_cleared` tinyint(1) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `user_teams`
--

CREATE TABLE `user_teams` (
  `id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `team_type` varchar(50) NOT NULL,
  `card_slot_index` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Chỉ mục cho các bảng đã đổ
--

--
-- Chỉ mục cho bảng `card_definitions`
--
ALTER TABLE `card_definitions`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `card_id` (`card_id`);

--
-- Chỉ mục cho bảng `endless_leaderboard`
--
ALTER TABLE `endless_leaderboard`
  ADD PRIMARY KEY (`user_id`),
  ADD KEY `idx_highest_floor` (`highest_floor`,`achieved_at`);

--
-- Chỉ mục cho bảng `owned_cards`
--
ALTER TABLE `owned_cards`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id` (`user_id`),
  ADD KEY `card_id` (`card_id`);

--
-- Chỉ mục cho bảng `stages`
--
ALTER TABLE `stages`
  ADD PRIMARY KEY (`stage_number`);

--
-- Chỉ mục cho bảng `stage_enemies`
--
ALTER TABLE `stage_enemies`
  ADD PRIMARY KEY (`id`),
  ADD KEY `stage_number` (`stage_number`),
  ADD KEY `card_id` (`card_id`);

--
-- Chỉ mục cho bảng `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- Chỉ mục cho bảng `user_stage_progress`
--
ALTER TABLE `user_stage_progress`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id` (`user_id`),
  ADD KEY `stage_number` (`stage_number`);

--
-- Chỉ mục cho bảng `user_teams`
--
ALTER TABLE `user_teams`
  ADD PRIMARY KEY (`id`),
  ADD KEY `user_id` (`user_id`);

--
-- AUTO_INCREMENT cho các bảng đã đổ
--

--
-- AUTO_INCREMENT cho bảng `card_definitions`
--
ALTER TABLE `card_definitions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=56;

--
-- AUTO_INCREMENT cho bảng `owned_cards`
--
ALTER TABLE `owned_cards`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=75;

--
-- AUTO_INCREMENT cho bảng `stage_enemies`
--
ALTER TABLE `stage_enemies`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=37;

--
-- AUTO_INCREMENT cho bảng `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=14;

--
-- AUTO_INCREMENT cho bảng `user_stage_progress`
--
ALTER TABLE `user_stage_progress`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT cho bảng `user_teams`
--
ALTER TABLE `user_teams`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- Các ràng buộc cho các bảng đã đổ
--

--
-- Các ràng buộc cho bảng `endless_leaderboard`
--
ALTER TABLE `endless_leaderboard`
  ADD CONSTRAINT `endless_leaderboard_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE;

--
-- Các ràng buộc cho bảng `owned_cards`
--
ALTER TABLE `owned_cards`
  ADD CONSTRAINT `owned_cards_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `owned_cards_ibfk_2` FOREIGN KEY (`card_id`) REFERENCES `card_definitions` (`card_id`) ON DELETE CASCADE;

--
-- Các ràng buộc cho bảng `stage_enemies`
--
ALTER TABLE `stage_enemies`
  ADD CONSTRAINT `fk_stage_enemies_stage` FOREIGN KEY (`stage_number`) REFERENCES `stages` (`stage_number`) ON DELETE CASCADE,
  ADD CONSTRAINT `fk_stage_enemies_card` FOREIGN KEY (`card_id`) REFERENCES `card_definitions` (`card_id`) ON DELETE CASCADE;

--
-- Các ràng buộc cho bảng `user_stage_progress`
--
ALTER TABLE `user_stage_progress`
  ADD CONSTRAINT `user_stage_progress_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `user_stage_progress_ibfk_2` FOREIGN KEY (`stage_number`) REFERENCES `stages` (`stage_number`) ON DELETE CASCADE;

--
-- Các ràng buộc cho bảng `user_teams`
--
ALTER TABLE `user_teams`
  ADD CONSTRAINT `user_teams_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;