SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Cấu trúc bảng cho bảng `card_definitions`
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
  `base_star` int(11) NOT NULL DEFAULT 1,
  `evolve_at_level` int(11) DEFAULT 0,
  `evolved_name` varchar(100) DEFAULT '',
  `evolve_atk_gain` int(11) DEFAULT 0,
  `evolve_hp_gain` int(11) DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

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
-- Cấu trúc bảng cho bảng `stages`
--

CREATE TABLE `stages` (
  `stage_number` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `enemy_bonus_hp` int(11) NOT NULL DEFAULT 0,
  `is_boss` tinyint(1) NOT NULL DEFAULT 0,
  `gold_reward` int(11) NOT NULL DEFAULT 30
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Cấu trúc bảng cho bảng `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `username` varchar(50) NOT NULL,
  `password` varchar(100) NOT NULL,
  `gold` int(11) NOT NULL DEFAULT 200,
  `gems` int(11) NOT NULL DEFAULT 10,
  `current_stage` int(11) NOT NULL DEFAULT 1,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  `last_login` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

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
-- Chỉ mục (Indexes) cho các bảng đã đổ
--

--
-- Chỉ mục cho bảng `card_definitions`
--
ALTER TABLE `card_definitions`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `card_id` (`card_id`);

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
-- Chỉ mục cho bảng `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- Chỉ mục cho bảng `endless_leaderboard`
--
ALTER TABLE `endless_leaderboard`
  ADD PRIMARY KEY (`user_id`),
  ADD KEY `idx_highest_floor` (`highest_floor` DESC, `achieved_at` ASC);

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
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=50;

--
-- AUTO_INCREMENT cho bảng `owned_cards`
--
ALTER TABLE `owned_cards`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=31;

--
-- AUTO_INCREMENT cho bảng `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

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
-- Các ràng buộc (Constraints) cho các bảng đã đổ
--

--
-- Các ràng buộc cho bảng `owned_cards`
--
ALTER TABLE `owned_cards`
  ADD CONSTRAINT `owned_cards_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `owned_cards_ibfk_2` FOREIGN KEY (`card_id`) REFERENCES `card_definitions` (`card_id`) ON DELETE CASCADE;

--
-- Các ràng buộc cho bảng `endless_leaderboard`
--
ALTER TABLE `endless_leaderboard`
  ADD CONSTRAINT `endless_leaderboard_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE;

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

UPDATE endless_leaderboard el
JOIN (
    -- Bọc thêm một lớp SELECT bên ngoài để tránh lỗi "Table is specified twice" (Lỗi #1093)
    SELECT user_id, calculated_rank 
    FROM (
        SELECT 
            user_id,
            RANK() OVER (ORDER BY highest_floor DESC, achieved_at ASC) as calculated_rank
        FROM endless_leaderboard
    ) AS temp
) rp ON el.user_id = rp.user_id
SET el.rank_position = rp.calculated_rank;