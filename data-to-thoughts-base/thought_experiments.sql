-- MySQL dump 10.13  Distrib 5.5.28, for solaris10 (sparc)
--
-- Host: ix    Database: thought_experiments
-- ------------------------------------------------------
-- Server version	5.5.28

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
-- Table structure for table `Comment`
--

DROP TABLE IF EXISTS `Comment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Comment` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` varchar(500) NOT NULL,
  `TE_id` int(11) DEFAULT NULL,
  `TS_id` int(11) DEFAULT NULL,
  `M_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  KEY `TE_id` (`TE_id`),
  KEY `TS_id` (`TS_id`),
  KEY `M_id` (`M_id`),
  CONSTRAINT `TE_id` FOREIGN KEY (`TE_id`) REFERENCES `ThoughtExperiment` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `TS_id` FOREIGN KEY (`TS_id`) REFERENCES `TextSection` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `M_id` FOREIGN KEY (`M_id`) REFERENCES `Multimedia` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Comment`
--

LOCK TABLES `Comment` WRITE;
/*!40000 ALTER TABLE `Comment` DISABLE KEYS */;
INSERT INTO `Comment` VALUES (1,'Not bad.',29,NULL,NULL),(2,'Great!',29,NULL,NULL),(3,'Great!',29,NULL,NULL),(4,'Kid looks tired.',NULL,NULL,21),(5,'Quite funny.',31,NULL,NULL),(6,'Misshapen leviathan is good.',NULL,25,NULL),(7,'Sensing common themes here.',35,NULL,NULL),(8,'I concur.',NULL,27,NULL),(9,'Say what?',36,NULL,NULL),(10,'Finally!',38,NULL,NULL),(11,'I\'m a big fan of these lyrics, and want to learn how to play this song.',NULL,28,NULL);
/*!40000 ALTER TABLE `Comment` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Mood`
--

DROP TABLE IF EXISTS `Mood`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Mood` (
  `name` varchar(20) NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Mood`
--

LOCK TABLES `Mood` WRITE;
/*!40000 ALTER TABLE `Mood` DISABLE KEYS */;
INSERT INTO `Mood` VALUES ('Angry'),('Energetic'),('Enraged'),('Excited'),('Happy'),('Inebriated'),('Inquisitive'),('Introspective'),('Joking'),('Sad'),('Sarcastic'),('Surprised'),('Thoughtful'),('Tired'),('Wistful');
/*!40000 ALTER TABLE `Mood` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Mult_Mood`
--

DROP TABLE IF EXISTS `Mult_Mood`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Mult_Mood` (
  `mood_name` varchar(20) NOT NULL,
  `mult_id` int(11) NOT NULL,
  PRIMARY KEY (`mood_name`,`mult_id`),
  KEY `M_name3` (`mood_name`),
  KEY `mult_id7` (`mult_id`),
  CONSTRAINT `M_name3` FOREIGN KEY (`mood_name`) REFERENCES `Mood` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `mult_id7` FOREIGN KEY (`mult_id`) REFERENCES `Multimedia` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Mult_Mood`
--

LOCK TABLES `Mult_Mood` WRITE;
/*!40000 ALTER TABLE `Mult_Mood` DISABLE KEYS */;
INSERT INTO `Mult_Mood` VALUES ('Joking',20),('Sarcastic',20),('Tired',21),('Excited',22),('Happy',22),('Inebriated',23),('Happy',24),('Joking',24),('Excited',25),('Inquisitive',25),('Introspective',25);
/*!40000 ALTER TABLE `Mult_Mood` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Multimedia`
--

DROP TABLE IF EXISTS `Multimedia`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Multimedia` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(100) NOT NULL,
  `type` varchar(5) NOT NULL,
  `location` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Multimedia`
--

LOCK TABLES `Multimedia` WRITE;
/*!40000 ALTER TABLE `Multimedia` DISABLE KEYS */;
INSERT INTO `Multimedia` VALUES (19,'Motivation','Image','http://www.matternow.com/wp-content/uploads/2013/01/motivation.jpg'),(20,'PHP','Image','http://tungpham42.info/dojo/wp-content/uploads/2012/04/php.png'),(21,'ZZZ','Image','http://blogmedia.eventbrite.com/wp-content/uploads/sleep-on-books-1.10.12.jpg'),(22,'reward','Image','http://2.bp.blogspot.com/-1xgz5HIm-I0/UAzsdHzmINI/AAAAAAAAMZA/VjIWEUjmV8U/s1600/rewards.jpg'),(23,'happy','Image','http://images3.wikia.nocookie.net/__cb20120604225106/creepypasta/images/8/88/Yellow_Happy.jpg'),(24,'Beatles','Image','http://static.tumblr.com/osuk4jz/tCCm4c3o4/wallpaper.jpg'),(25,'Idea','Image','http://kennybriscoe.com/wp-content/uploads/2011/02/lightbulb-idea.jpg'),(26,'tiger1','Image','http://25.media.tumblr.com/tumblr_m6fmnhxL3B1r7wu2mo1_500.jpg'),(27,'tiger2','Image','http://goodnature.nathab.com/wp-content/uploads/2010/10/Bengal_Tiger1.jpg'),(28,'tiger3','Image','http://1.bp.blogspot.com/-e_yASjMmkLQ/T70A2i01bBI/AAAAAAAAAwc/3Mo40TpXtB0/s1600/tiger.jpeg');
/*!40000 ALTER TABLE `Multimedia` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TE_Cat`
--

DROP TABLE IF EXISTS `TE_Cat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TE_Cat` (
  `cat_name` varchar(20) NOT NULL,
  `TE_id` int(11) NOT NULL,
  PRIMARY KEY (`TE_id`,`cat_name`),
  KEY `cat_name2` (`cat_name`),
  KEY `TE_id2` (`TE_id`),
  CONSTRAINT `cat_name2` FOREIGN KEY (`cat_name`) REFERENCES `ThoughtCategory` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `TE_id2` FOREIGN KEY (`TE_id`) REFERENCES `ThoughtExperiment` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TE_Cat`
--

LOCK TABLES `TE_Cat` WRITE;
/*!40000 ALTER TABLE `TE_Cat` DISABLE KEYS */;
INSERT INTO `TE_Cat` VALUES ('Todo',29),('Poem',30),('Blog',31),('Note',32),('Song',33),('Note',34),('Idea',35),('Story',36),('Uncategorized',37),('Journal',38),('Uncategorized',39);
/*!40000 ALTER TABLE `TE_Cat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TE_Mood`
--

DROP TABLE IF EXISTS `TE_Mood`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TE_Mood` (
  `mood_name` varchar(20) NOT NULL,
  `TE_id` int(11) NOT NULL,
  PRIMARY KEY (`mood_name`,`TE_id`),
  KEY `mood_name` (`mood_name`),
  KEY `TE_id5` (`TE_id`),
  CONSTRAINT `mood_name` FOREIGN KEY (`mood_name`) REFERENCES `Mood` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `TE_id5` FOREIGN KEY (`TE_id`) REFERENCES `ThoughtExperiment` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TE_Mood`
--

LOCK TABLES `TE_Mood` WRITE;
/*!40000 ALTER TABLE `TE_Mood` DISABLE KEYS */;
INSERT INTO `TE_Mood` VALUES ('Energetic',29),('Joking',29),('Tired',29),('Angry',34),('Enraged',34),('Joking',34),('Inebriated',36),('Sarcastic',36),('Energetic',39),('Inquisitive',39),('Surprised',39);
/*!40000 ALTER TABLE `TE_Mood` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TE_Mult`
--

DROP TABLE IF EXISTS `TE_Mult`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TE_Mult` (
  `mult_id` int(11) NOT NULL,
  `TE_id` int(11) NOT NULL,
  PRIMARY KEY (`TE_id`,`mult_id`),
  KEY `mult_id2` (`mult_id`),
  KEY `TE_id3` (`TE_id`),
  CONSTRAINT `mult_id` FOREIGN KEY (`mult_id`) REFERENCES `Multimedia` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `TE_id3` FOREIGN KEY (`TE_id`) REFERENCES `ThoughtExperiment` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TE_Mult`
--

LOCK TABLES `TE_Mult` WRITE;
/*!40000 ALTER TABLE `TE_Mult` DISABLE KEYS */;
INSERT INTO `TE_Mult` VALUES (19,29),(20,30),(21,32),(22,32),(23,32),(24,33),(25,35),(26,39),(27,39),(28,39);
/*!40000 ALTER TABLE `TE_Mult` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TE_TextSec`
--

DROP TABLE IF EXISTS `TE_TextSec`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TE_TextSec` (
  `TS_id` int(11) NOT NULL,
  `TE_id` int(11) NOT NULL,
  PRIMARY KEY (`TE_id`,`TS_id`),
  KEY `TS_id` (`TS_id`),
  KEY `TE_id4` (`TE_id`),
  CONSTRAINT `TS_id2` FOREIGN KEY (`TS_id`) REFERENCES `TextSection` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `TE_id4` FOREIGN KEY (`TE_id`) REFERENCES `ThoughtExperiment` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TE_TextSec`
--

LOCK TABLES `TE_TextSec` WRITE;
/*!40000 ALTER TABLE `TE_TextSec` DISABLE KEYS */;
INSERT INTO `TE_TextSec` VALUES (24,29),(25,30),(26,31),(27,32),(28,33),(29,34),(30,35),(31,36),(32,37),(33,38),(34,39);
/*!40000 ALTER TABLE `TE_TextSec` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TS_Mood`
--

DROP TABLE IF EXISTS `TS_Mood`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TS_Mood` (
  `mood_name` varchar(20) NOT NULL,
  `TS_id` int(11) NOT NULL,
  PRIMARY KEY (`mood_name`,`TS_id`),
  KEY `mood_name2` (`mood_name`),
  KEY `TS_id6` (`TS_id`),
  CONSTRAINT `mood_name2` FOREIGN KEY (`mood_name`) REFERENCES `Mood` (`name`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `TS_id6` FOREIGN KEY (`TS_id`) REFERENCES `TextSection` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TS_Mood`
--

LOCK TABLES `TS_Mood` WRITE;
/*!40000 ALTER TABLE `TS_Mood` DISABLE KEYS */;
INSERT INTO `TS_Mood` VALUES ('Energetic',24),('Joking',24),('Sarcastic',24),('Joking',26),('Thoughtful',26),('Tired',26),('Tired',27),('Thoughtful',28),('Tired',28),('Wistful',28),('Thoughtful',32);
/*!40000 ALTER TABLE `TS_Mood` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TextCategory`
--

DROP TABLE IF EXISTS `TextCategory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TextCategory` (
  `name` varchar(20) NOT NULL,
  PRIMARY KEY (`name`),
  UNIQUE KEY `name` (`name`),
  UNIQUE KEY `id_UNIQUE` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TextCategory`
--

LOCK TABLES `TextCategory` WRITE;
/*!40000 ALTER TABLE `TextCategory` DISABLE KEYS */;
INSERT INTO `TextCategory` VALUES ('Lyric'),('Poetry'),('Prose'),('Question'),('Quote'),('Ramble'),('Reflection'),('Statement'),('Thought'),('Uncategorized');
/*!40000 ALTER TABLE `TextCategory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `TextSection`
--

DROP TABLE IF EXISTS `TextSection`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `TextSection` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` varchar(1000) DEFAULT NULL,
  `date_created` datetime DEFAULT NULL,
  `date_modified` datetime NOT NULL,
  `cat_name` varchar(20) DEFAULT 'Uncategorized',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  KEY `cat_name` (`cat_name`),
  CONSTRAINT `cat_name` FOREIGN KEY (`cat_name`) REFERENCES `TextCategory` (`name`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `TextSection`
--

LOCK TABLES `TextSection` WRITE;
/*!40000 ALTER TABLE `TextSection` DISABLE KEYS */;
INSERT INTO `TextSection` VALUES (24,'Get this assignment done!','2013-03-20 11:05:28','2013-03-20 11:05:28','Thought'),(25,'PHP how hideous you are,\r\nMisshapen leviathan of giddy Frito-fingered dreams,\r\nSwamp man of the web.','2013-03-20 11:49:19','2013-03-20 11:49:19','Poetry'),(26,'When one pushes the outer boundaries of sleep deprivation, one finds extreme discomfort, irritability, a depressingly brief period of psychosomatic giddiness, paranoia, mumbled speech that could confuse a drunk, and drug-like stupor.  But near the bottom of this slurried nerve-death cocktail, we find a neat little bit of hallucinogenic-esque enlightenment, the pomegranate seeds swimming the aesthete\'s glass of acid.  But I\'m not there yet, right now--in fact I\'m far, far away from that point.','2013-03-20 12:10:03','2013-03-20 12:10:03','Ramble'),(27,'Sleep some.  You\'ll feel good.','2013-03-20 14:07:55','2013-03-20 14:07:55','Quote'),(28,'I once had a girl, or should I say, she once had me... \r\nShe showed me her room, isn\'t it good, norwegian wood? \r\n\r\nShe asked me to stay and she told me to sit anywhere, \r\nSo I looked around and I noticed there wasn\'t a chair. \r\n\r\nI sat on a rug, biding my time, drinking her wine \r\nWe talked until two and then she said, \"It\'s time for bed\" \r\n\r\nShe told me she worked in the morning and started to laugh. \r\nI told her I didn\'t and crawled off to sleep in the bath \r\n\r\nAnd when I awoke, I was alone, this bird had flown \r\nSo I lit a fire, isn\'t it good, norwegian wood.','2013-03-20 14:15:06','2013-03-20 14:15:06','Lyric'),(29,'Finish the project already!','2013-03-20 14:16:29','2013-03-20 14:16:29','Uncategorized'),(30,'Let\'s find a way to make glasses that measure REM sleep, and give you a soft alarm when you\'ve REM\'ed.','2013-03-20 14:17:59','2013-03-20 14:17:59','Thought'),(31,'Don\'t Ask.  I won\'t tell.','2013-03-20 14:18:52','2013-03-20 14:18:52','Uncategorized'),(32,'I kind of like this project.  I may elaborate on it and try to use it someday.','2013-03-20 14:19:40','2013-03-20 14:19:40','Prose'),(33,'Upon looking back, I conclude:\r\n1. I\'m tired;\r\n2. I\'m loopy;\r\n3. I need sleep.','2013-03-20 14:20:21','2013-03-20 14:20:21','Reflection'),(34,'Whoa','2013-03-20 14:35:30','2013-03-20 14:35:30','Uncategorized');
/*!40000 ALTER TABLE `TextSection` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ThoughtCategory`
--

DROP TABLE IF EXISTS `ThoughtCategory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ThoughtCategory` (
  `name` varchar(20) NOT NULL,
  PRIMARY KEY (`name`),
  UNIQUE KEY `id_UNIQUE` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ThoughtCategory`
--

LOCK TABLES `ThoughtCategory` WRITE;
/*!40000 ALTER TABLE `ThoughtCategory` DISABLE KEYS */;
INSERT INTO `ThoughtCategory` VALUES ('Blog'),('Idea'),('Journal'),('List'),('Note'),('Poem'),('Song'),('Story'),('Todo'),('Uncategorized');
/*!40000 ALTER TABLE `ThoughtCategory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ThoughtExperiment`
--

DROP TABLE IF EXISTS `ThoughtExperiment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ThoughtExperiment` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(50) DEFAULT NULL,
  `date_created` datetime NOT NULL,
  `date_modified` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=40 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ThoughtExperiment`
--

LOCK TABLES `ThoughtExperiment` WRITE;
/*!40000 ALTER TABLE `ThoughtExperiment` DISABLE KEYS */;
INSERT INTO `ThoughtExperiment` VALUES (29,'First Thought','2013-03-20 11:05:28','2013-03-20 11:05:28'),(30,'Another Mess','2013-03-20 11:49:19','2013-03-20 11:49:19'),(31,'Sleep deprivation is a B****','2013-03-20 12:10:03','2013-03-20 12:10:03'),(32,'Sleep','2013-03-20 14:07:55','2013-03-20 14:07:55'),(33,'Norwegian Wood-good song','2013-03-20 14:15:06','2013-03-20 14:15:06'),(34,'Quick','2013-03-20 14:16:29','2013-03-20 14:16:29'),(35,'How About REM-glasses?','2013-03-20 14:17:59','2013-03-20 14:17:59'),(36,'Dancing Queen','2013-03-20 14:18:52','2013-03-20 14:18:52'),(37,'Almost Done','2013-03-20 14:19:40','2013-03-20 14:19:40'),(38,'Last Entry','2013-03-20 14:20:21','2013-03-20 14:20:21'),(39,'Tiga\'s \'S Crazy','2013-03-20 14:35:30','2013-03-20 14:35:30');
/*!40000 ALTER TABLE `ThoughtExperiment` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2013-03-20  7:41:15
