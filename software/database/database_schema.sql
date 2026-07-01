-- =========================================================
-- Smart Home Database Schema
-- MariaDB 10.5+
--
-- Database schema for the Smart Home project.
-- Runtime data, user settings and measurements are intentionally excluded.
-- =========================================================

CREATE DATABASE IF NOT EXISTS `s_h`
    DEFAULT CHARACTER SET utf8
    COLLATE utf8_general_ci;

USE `s_h`;

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;

--
-- Alarm System State
--
CREATE TABLE a_s (
  id int(2) NOT NULL,
  name varchar(50) NOT NULL,
  state varchar(3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Alarm_State';

--
-- Blackout scenario configuration
--
CREATE TABLE b_o (
  id int(3) NOT NULL,
  number varchar(30) NOT NULL,
  name varchar(30) NOT NULL,
  state varchar(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Blackout function';

--
-- USSD-requests and answers
--
CREATE TABLE gsm (
  id int(3) NOT NULL,
  name varchar(20) NOT NULL,
  value varchar(80) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='USSD-requests';

--
-- Simplified lighting control panel state
--
CREATE TABLE lls (
  id int(11) NOT NULL,
  number int(4) NOT NULL,
  name varchar(32) NOT NULL,
  state varchar(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Local_Lighting_State';

--
-- Detailed lighting control panel state
--
CREATE TABLE msp (
  id int(11) NOT NULL,
  number varchar(8) NOT NULL,
  name varchar(48) NOT NULL,
  state varchar(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Manual_Switch_Panel';

--
--  Feature configuration switches
--
CREATE TABLE m_s (
  id int(11) NOT NULL,
  number varchar(15) NOT NULL,
  name varchar(80) NOT NULL,
  state varchar(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Management_Switchers (Settings)';

--
-- Power and environmental monitoring
--
CREATE TABLE p_m (
  id int(3) NOT NULL,
  name varchar(54) NOT NULL,
  value varchar(6) NOT NULL,
  unit varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Power Management';

--
-- Timer Settings and Delay Times for Scenarios
--
CREATE TABLE t_v (
  id int(3) NOT NULL,
  name varchar(30) NOT NULL,
  value varchar(10) NOT NULL,
  unit varchar(4) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_general_ci COMMENT='Timer''s values';

--
-- Primary keys
--
ALTER TABLE a_s
  ADD PRIMARY KEY (id);

ALTER TABLE b_o
  ADD PRIMARY KEY (id);

ALTER TABLE gsm
  ADD PRIMARY KEY (id);

ALTER TABLE lls
  ADD PRIMARY KEY (id);

ALTER TABLE msp
  ADD PRIMARY KEY (id);

ALTER TABLE m_s
  ADD PRIMARY KEY (id);

ALTER TABLE p_m
  ADD PRIMARY KEY (id);

ALTER TABLE t_v
  ADD PRIMARY KEY (id);

--
-- AUTO_INCREMENT
--
ALTER TABLE a_s
  MODIFY id int(2) NOT NULL AUTO_INCREMENT;

ALTER TABLE b_o
  MODIFY id int(3) NOT NULL AUTO_INCREMENT;

ALTER TABLE gsm
  MODIFY id int(3) NOT NULL AUTO_INCREMENT;

ALTER TABLE lls
  MODIFY id int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE msp
  MODIFY id int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE m_s
  MODIFY id int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE p_m
  MODIFY id int(3) NOT NULL AUTO_INCREMENT;

ALTER TABLE t_v
  MODIFY id int(3) NOT NULL AUTO_INCREMENT;
COMMIT;

