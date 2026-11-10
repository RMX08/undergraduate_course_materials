-- MySQL dump 10.13  Distrib 8.0.39, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: education_system
-- ------------------------------------------------------
-- Server version	8.0.39

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `class`
--

DROP TABLE IF EXISTS `class`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `class` (
  `cs_id` int NOT NULL COMMENT '班级编号',
  `cs_number` int NOT NULL COMMENT '班级人数',
  `cs_instructor` varchar(20) NOT NULL COMMENT '辅导员',
  `col_id` int NOT NULL COMMENT '所属院系',
  PRIMARY KEY (`cs_id`),
  KEY `col_id` (`col_id`),
  CONSTRAINT `class_ibfk_1` FOREIGN KEY (`col_id`) REFERENCES `college` (`col_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='班级';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `class`
--

LOCK TABLES `class` WRITE;
/*!40000 ALTER TABLE `class` DISABLE KEYS */;
INSERT INTO `class` VALUES (1,48,'王小菲',3),(2,101,'乔伊',1),(3,69,'赵任凡',4);
/*!40000 ALTER TABLE `class` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `college`
--

DROP TABLE IF EXISTS `college`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `college` (
  `col_id` int NOT NULL COMMENT '学院编号',
  `col_name` varchar(20) NOT NULL COMMENT '学院名称',
  `campus` varchar(20) NOT NULL COMMENT '所在校区',
  PRIMARY KEY (`col_id`),
  UNIQUE KEY `col_name` (`col_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='学院';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `college`
--

LOCK TABLES `college` WRITE;
/*!40000 ALTER TABLE `college` DISABLE KEYS */;
INSERT INTO `college` VALUES (1,'信息与工程学部','西海岸校区'),(2,'水产学院','鱼山校区 '),(3,'法学院','崂山校区'),(4,'食品与工程学院','西海岸校区');
/*!40000 ALTER TABLE `college` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `course`
--

DROP TABLE IF EXISTS `course`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `course` (
  `crs_id` int NOT NULL COMMENT '课程编号',
  `crs_name` varchar(20) NOT NULL COMMENT '课程名称',
  `crs_point` int NOT NULL COMMENT '课程学分',
  `crs_date` varchar(30) NOT NULL COMMENT '上课时间',
  `col_id` int NOT NULL COMMENT '所属院系',
  PRIMARY KEY (`crs_id`),
  KEY `col_id` (`col_id`),
  CONSTRAINT `course_ibfk_1` FOREIGN KEY (`col_id`) REFERENCES `college` (`col_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='课程';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `course`
--

LOCK TABLES `course` WRITE;
/*!40000 ALTER TABLE `course` DISABLE KEYS */;
INSERT INTO `course` VALUES (1,'数据库系统',3,'周二3-4 周三5-6',1),(2,'国际秩序与条约',2,'周一3-4',3),(3,'养殖技术',3,'周四1-2',2);
/*!40000 ALTER TABLE `course` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gender`
--

DROP TABLE IF EXISTS `gender`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `gender` (
  `gd_id` int NOT NULL COMMENT '性别编号',
  `gd_name` varchar(10) NOT NULL COMMENT '性别',
  PRIMARY KEY (`gd_id`),
  UNIQUE KEY `gd_name` (`gd_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='性别';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gender`
--

LOCK TABLES `gender` WRITE;
/*!40000 ALTER TABLE `gender` DISABLE KEYS */;
INSERT INTO `gender` VALUES (3,'你猜'),(1,'女'),(2,'男');
/*!40000 ALTER TABLE `gender` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graduate_student`
--

DROP TABLE IF EXISTS `graduate_student`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `graduate_student` (
  `gs_id` int NOT NULL COMMENT '研究生学号',
  `gs_name` varchar(20) NOT NULL COMMENT '研究生姓名',
  `gender` int NOT NULL COMMENT '性别',
  `tc_id` int NOT NULL COMMENT '导师',
  `col_id` int NOT NULL COMMENT '所属院系',
  PRIMARY KEY (`gs_id`),
  KEY `tc_id` (`tc_id`),
  KEY `col_id` (`col_id`),
  CONSTRAINT `graduate_student_ibfk_1` FOREIGN KEY (`tc_id`) REFERENCES `teacher` (`tc_id`) ON UPDATE CASCADE,
  CONSTRAINT `graduate_student_ibfk_2` FOREIGN KEY (`col_id`) REFERENCES `college` (`col_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='研究生';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `graduate_student`
--

LOCK TABLES `graduate_student` WRITE;
/*!40000 ALTER TABLE `graduate_student` DISABLE KEYS */;
INSERT INTO `graduate_student` VALUES (1,'吴越',2,3,1),(2,'周倩',1,2,2),(3,'胡想',3,2,1);
/*!40000 ALTER TABLE `graduate_student` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `student`
--

DROP TABLE IF EXISTS `student`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `student` (
  `stu_id` int NOT NULL COMMENT '学号',
  `stu_name` varchar(20) NOT NULL COMMENT '学生姓名',
  `gender` int NOT NULL COMMENT '性别',
  `col_id` int NOT NULL COMMENT '所属院系',
  `cs_id` int NOT NULL COMMENT '班级号',
  `AvgGrade` decimal(4,1) DEFAULT '0.0' COMMENT '平均成绩',
  PRIMARY KEY (`stu_id`),
  KEY `col_id` (`col_id`),
  KEY `cs_id` (`cs_id`),
  CONSTRAINT `student_ibfk_1` FOREIGN KEY (`col_id`) REFERENCES `college` (`col_id`) ON UPDATE CASCADE,
  CONSTRAINT `student_ibfk_2` FOREIGN KEY (`cs_id`) REFERENCES `class` (`cs_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='学生';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `student`
--

LOCK TABLES `student` WRITE;
/*!40000 ALTER TABLE `student` DISABLE KEYS */;
INSERT INTO `student` VALUES (1,'秦英',1,2,1,92.8),(2,'晓晓',1,3,2,73.2),(3,'大王',3,4,3,0.0),(4,'小王',2,2,1,0.0);
/*!40000 ALTER TABLE `student` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `student_instructor`
--

DROP TABLE IF EXISTS `student_instructor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `student_instructor` (
  `stu_id` int NOT NULL COMMENT '学号',
  `cs_id` int DEFAULT NULL COMMENT '班级号',
  `instructor` varchar(20) DEFAULT NULL COMMENT '辅导员姓名',
  PRIMARY KEY (`stu_id`),
  KEY `student_instructor` (`cs_id`),
  CONSTRAINT `student_instructor_ibfk_1` FOREIGN KEY (`stu_id`) REFERENCES `student` (`stu_id`),
  CONSTRAINT `student_instructor_ibfk_2` FOREIGN KEY (`cs_id`) REFERENCES `class` (`cs_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='学生-辅导员 2NF';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `student_instructor`
--

LOCK TABLES `student_instructor` WRITE;
/*!40000 ALTER TABLE `student_instructor` DISABLE KEYS */;
INSERT INTO `student_instructor` VALUES (1,1,'王小菲'),(2,2,'乔英'),(4,1,'王小菲');
/*!40000 ALTER TABLE `student_instructor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `take_course`
--

DROP TABLE IF EXISTS `take_course`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `take_course` (
  `tcrs_id` int NOT NULL COMMENT '选课编号',
  `stu_id` int NOT NULL COMMENT '学生学号',
  `crs_id` int NOT NULL COMMENT '课程编号',
  `grade` decimal(3,1) NOT NULL COMMENT '成绩',
  PRIMARY KEY (`tcrs_id`),
  KEY `stu_id` (`stu_id`),
  KEY `crs_id` (`crs_id`),
  CONSTRAINT `take_course_ibfk_1` FOREIGN KEY (`stu_id`) REFERENCES `student` (`stu_id`) ON UPDATE CASCADE,
  CONSTRAINT `take_course_ibfk_2` FOREIGN KEY (`crs_id`) REFERENCES `course` (`crs_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='选修';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `take_course`
--

LOCK TABLES `take_course` WRITE;
/*!40000 ALTER TABLE `take_course` DISABLE KEYS */;
INSERT INTO `take_course` VALUES (1,1,1,91.0),(2,1,2,89.5),(3,2,3,73.2),(4,1,3,98.0);
/*!40000 ALTER TABLE `take_course` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_after_insert_grade */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `trg_after_insert_grade` AFTER INSERT ON `take_course` FOR EACH ROW BEGIN
    UPDATE student
    SET AvgGrade = (SELECT AVG(grade) FROM take_course WHERE stu_id = NEW.stu_id)
    WHERE stu_id = NEW.stu_id;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
/*!50032 DROP TRIGGER IF EXISTS trg_after_update_grade */;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `trg_after_update_grade` AFTER UPDATE ON `take_course` FOR EACH ROW BEGIN
    -- 只有当成绩字段发生变化时才更新，节省开销
    IF OLD.grade <> NEW.grade THEN
        UPDATE student
        SET AvgGrade = (SELECT AVG(grade) FROM take_course WHERE stu_id = NEW.stu_id)
        WHERE stu_id = NEW.stu_id;
    END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `teacher`
--

DROP TABLE IF EXISTS `teacher`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `teacher` (
  `tc_id` int NOT NULL COMMENT '教员工号',
  `tc_name` varchar(10) NOT NULL COMMENT '教员姓名',
  `tc_title` int NOT NULL COMMENT '教员职称',
  `phone` varchar(11) NOT NULL COMMENT '联系方式',
  `tr_id` int NOT NULL COMMENT '教研室编号',
  `gender` int DEFAULT NULL,
  PRIMARY KEY (`tc_id`),
  KEY `tc_title` (`tc_title`),
  KEY `tr_id` (`tr_id`),
  CONSTRAINT `teacher_ibfk_1` FOREIGN KEY (`tc_title`) REFERENCES `title` (`tt_id`) ON UPDATE CASCADE,
  CONSTRAINT `teacher_ibfk_2` FOREIGN KEY (`tr_id`) REFERENCES `teaching_and_research_office` (`tr_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='教员';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `teacher`
--

LOCK TABLES `teacher` WRITE;
/*!40000 ALTER TABLE `teacher` DISABLE KEYS */;
INSERT INTO `teacher` VALUES (1,'张叁',2,'15816483540',3,2),(2,'李斯',1,'16418982550',2,2),(3,'王武',4,'15548770520',1,2);
/*!40000 ALTER TABLE `teacher` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `teacher_project_guide`
--

DROP TABLE IF EXISTS `teacher_project_guide`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `teacher_project_guide` (
  `stu_id` int NOT NULL,
  `tc_id` int NOT NULL,
  `col_id` int NOT NULL,
  PRIMARY KEY (`stu_id`),
  KEY `tc_id` (`tc_id`),
  KEY `col_id` (`col_id`),
  CONSTRAINT `teacher_project_guide_ibfk_1` FOREIGN KEY (`stu_id`) REFERENCES `student` (`stu_id`),
  CONSTRAINT `teacher_project_guide_ibfk_2` FOREIGN KEY (`tc_id`) REFERENCES `teacher` (`tc_id`),
  CONSTRAINT `teacher_project_guide_ibfk_3` FOREIGN KEY (`col_id`) REFERENCES `college` (`col_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='3NF非BCNF实验表：决定因素不全是候选码';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `teacher_project_guide`
--

LOCK TABLES `teacher_project_guide` WRITE;
/*!40000 ALTER TABLE `teacher_project_guide` DISABLE KEYS */;
/*!40000 ALTER TABLE `teacher_project_guide` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `teaching_and_research_office`
--

DROP TABLE IF EXISTS `teaching_and_research_office`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `teaching_and_research_office` (
  `tr_id` int NOT NULL COMMENT '教研室编号',
  `tr_name` varchar(20) NOT NULL COMMENT '教研室名',
  `col_id` int NOT NULL COMMENT '所属院系',
  PRIMARY KEY (`tr_id`),
  UNIQUE KEY `tr_name` (`tr_name`),
  KEY `col_id` (`col_id`),
  CONSTRAINT `teaching_and_research_office_ibfk_1` FOREIGN KEY (`col_id`) REFERENCES `college` (`col_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='教研室';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `teaching_and_research_office`
--

LOCK TABLES `teaching_and_research_office` WRITE;
/*!40000 ALTER TABLE `teaching_and_research_office` DISABLE KEYS */;
INSERT INTO `teaching_and_research_office` VALUES (1,'人工智能虚拟教研室',1),(2,'食品与工程专业虚拟教研室',4),(3,'水产养殖学专业虚拟教研室',2);
/*!40000 ALTER TABLE `teaching_and_research_office` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `title`
--

DROP TABLE IF EXISTS `title`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `title` (
  `tt_id` int NOT NULL COMMENT '职称编号',
  `tt_name` varchar(10) NOT NULL COMMENT '职称',
  PRIMARY KEY (`tt_id`),
  UNIQUE KEY `tt_name` (`tt_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='职称';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `title`
--

LOCK TABLES `title` WRITE;
/*!40000 ALTER TABLE `title` DISABLE KEYS */;
INSERT INTO `title` VALUES (1,'副教授'),(3,'助教'),(2,'教授'),(4,'讲师');
/*!40000 ALTER TABLE `title` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'education_system'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-01-14 19:02:53
