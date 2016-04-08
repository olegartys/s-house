-- MySQL dump 10.13  Distrib 5.6.28, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: DataStorage
-- ------------------------------------------------------
-- Server version	5.6.28-0ubuntu0.15.10.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `BinaryType`
--

DROP TABLE IF EXISTS `BinaryType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `BinaryType` (
  `systemSensorName` varchar(50) DEFAULT NULL,
  `type` varchar(50) DEFAULT NULL,
  `FA_id` varchar(50) DEFAULT NULL,
  `state` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `BinaryType`
--

LOCK TABLES `BinaryType` WRITE;
/*!40000 ALTER TABLE `BinaryType` DISABLE KEYS */;
INSERT INTO `BinaryType` VALUES ('light1','light','fa1','ON');
/*!40000 ALTER TABLE `BinaryType` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `MainTable`
--

DROP TABLE IF EXISTS `MainTable`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `MainTable` (
  `userSensorName` varchar(50) DEFAULT NULL,
  `systemSensorName` varchar(50) DEFAULT NULL,
  `sensorType` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `MainTable`
--

LOCK TABLES `MainTable` WRITE;
/*!40000 ALTER TABLE `MainTable` DISABLE KEYS */;
INSERT INTO `MainTable` VALUES ('sas',NULL,NULL),('sas',NULL,NULL),('DDsas',NULL,NULL),('DDsssas',NULL,NULL),('kek',NULL,NULL),('kek','kek','kek'),('sos','sos','sos'),('MyLight','light1','BinaryType');
/*!40000 ALTER TABLE `MainTable` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ManyStatesType`
--

DROP TABLE IF EXISTS `ManyStatesType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ManyStatesType` (
  `systemSensorName` varchar(50) DEFAULT NULL,
  `type` varchar(50) DEFAULT NULL,
  `FA_id` varchar(50) DEFAULT NULL,
  `state` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ManyStatesType`
--

LOCK TABLES `ManyStatesType` WRITE;
/*!40000 ALTER TABLE `ManyStatesType` DISABLE KEYS */;
/*!40000 ALTER TABLE `ManyStatesType` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `MonitorType`
--

DROP TABLE IF EXISTS `MonitorType`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `MonitorType` (
  `systemSensorName` varchar(50) DEFAULT NULL,
  `type` varchar(50) DEFAULT NULL,
  `FA_id` varchar(50) DEFAULT NULL,
  `state` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `MonitorType`
--

LOCK TABLES `MonitorType` WRITE;
/*!40000 ALTER TABLE `MonitorType` DISABLE KEYS */;
/*!40000 ALTER TABLE `MonitorType` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-04-08  9:46:35
