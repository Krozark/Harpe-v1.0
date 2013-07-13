-- phpMyAdmin SQL Dump
-- version 3.5.8.1deb1
-- http://www.phpmyadmin.net
--
-- Client: localhost
-- Généré le: Sam 13 Juillet 2013 à 02:50
-- Version du serveur: 5.5.31-0ubuntu0.13.04.1
-- Version de PHP: 5.4.9-4ubuntu2.1

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de données: `Harpe`
--

-- --------------------------------------------------------

--
-- Structure de la table `auth_group`
--

CREATE TABLE IF NOT EXISTS `auth_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(80) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `auth_group_permissions`
--

CREATE TABLE IF NOT EXISTS `auth_group_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `group_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `group_id` (`group_id`,`permission_id`),
  KEY `auth_group_permissions_bda51c3c` (`group_id`),
  KEY `auth_group_permissions_1e014c8f` (`permission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `auth_permission`
--

CREATE TABLE IF NOT EXISTS `auth_permission` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `content_type_id` int(11) NOT NULL,
  `codename` varchar(100) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `content_type_id` (`content_type_id`,`codename`),
  KEY `auth_permission_e4470c6e` (`content_type_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=31 ;

--
-- Contenu de la table `auth_permission`
--

INSERT INTO `auth_permission` (`id`, `name`, `content_type_id`, `codename`) VALUES
(1, 'Can add permission', 1, 'add_permission'),
(2, 'Can change permission', 1, 'change_permission'),
(3, 'Can delete permission', 1, 'delete_permission'),
(4, 'Can add group', 2, 'add_group'),
(5, 'Can change group', 2, 'change_group'),
(6, 'Can delete group', 2, 'delete_group'),
(7, 'Can add user', 3, 'add_user'),
(8, 'Can change user', 3, 'change_user'),
(9, 'Can delete user', 3, 'delete_user'),
(10, 'Can add content type', 4, 'add_contenttype'),
(11, 'Can change content type', 4, 'change_contenttype'),
(12, 'Can delete content type', 4, 'delete_contenttype'),
(13, 'Can add session', 5, 'add_session'),
(14, 'Can change session', 5, 'change_session'),
(15, 'Can delete session', 5, 'delete_session'),
(16, 'Can add site', 6, 'add_site'),
(17, 'Can change site', 6, 'change_site'),
(18, 'Can delete site', 6, 'delete_site'),
(19, 'Can add log entry', 7, 'add_logentry'),
(20, 'Can change log entry', 7, 'change_logentry'),
(21, 'Can delete log entry', 7, 'delete_logentry'),
(22, 'Can add amino acid', 8, 'add_aa'),
(23, 'Can change amino acid', 8, 'change_aa'),
(24, 'Can delete amino acid', 8, 'delete_aa'),
(25, 'Can add impossible cut', 9, 'add_impossiblecut'),
(26, 'Can change impossible cut', 9, 'change_impossiblecut'),
(27, 'Can delete impossible cut', 9, 'delete_impossiblecut'),
(28, 'Can add enzyme', 10, 'add_enzyme'),
(29, 'Can change enzyme', 10, 'change_enzyme'),
(30, 'Can delete enzyme', 10, 'delete_enzyme');

-- --------------------------------------------------------

--
-- Structure de la table `auth_user`
--

CREATE TABLE IF NOT EXISTS `auth_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) NOT NULL,
  `first_name` varchar(30) NOT NULL,
  `last_name` varchar(30) NOT NULL,
  `email` varchar(75) NOT NULL,
  `password` varchar(128) NOT NULL,
  `is_staff` tinyint(1) NOT NULL,
  `is_active` tinyint(1) NOT NULL,
  `is_superuser` tinyint(1) NOT NULL,
  `last_login` datetime NOT NULL,
  `date_joined` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Contenu de la table `auth_user`
--

INSERT INTO `auth_user` (`id`, `username`, `first_name`, `last_name`, `email`, `password`, `is_staff`, `is_active`, `is_superuser`, `last_login`, `date_joined`) VALUES
(1, 'admin', '', '', 'a@a.com', 'pbkdf2_sha256$10000$AiktCPwfnwQh$dIgBcpcdqDLOs9YpCLLwDYGiMpdZelVR6Tj7+Gie0hk=', 1, 1, 1, '2013-03-13 19:16:44', '2013-02-20 15:19:02');

-- --------------------------------------------------------

--
-- Structure de la table `auth_user_groups`
--

CREATE TABLE IF NOT EXISTS `auth_user_groups` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_id` (`user_id`,`group_id`),
  KEY `auth_user_groups_fbfc09f1` (`user_id`),
  KEY `auth_user_groups_bda51c3c` (`group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `auth_user_user_permissions`
--

CREATE TABLE IF NOT EXISTS `auth_user_user_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `user_id` (`user_id`,`permission_id`),
  KEY `auth_user_user_permissions_fbfc09f1` (`user_id`),
  KEY `auth_user_user_permissions_1e014c8f` (`permission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `django_admin_log`
--

CREATE TABLE IF NOT EXISTS `django_admin_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `action_time` datetime NOT NULL,
  `user_id` int(11) NOT NULL,
  `content_type_id` int(11) DEFAULT NULL,
  `object_id` longtext,
  `object_repr` varchar(200) NOT NULL,
  `action_flag` smallint(5) unsigned NOT NULL,
  `change_message` longtext NOT NULL,
  PRIMARY KEY (`id`),
  KEY `django_admin_log_fbfc09f1` (`user_id`),
  KEY `django_admin_log_e4470c6e` (`content_type_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Contenu de la table `django_admin_log`
--

INSERT INTO `django_admin_log` (`id`, `action_time`, `user_id`, `content_type_id`, `object_id`, `object_repr`, `action_flag`, `change_message`) VALUES
(1, '2013-03-13 19:16:53', 1, 8, '11', 'I_L', 2, 'Changed slug.');

-- --------------------------------------------------------

--
-- Structure de la table `django_content_type`
--

CREATE TABLE IF NOT EXISTS `django_content_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `app_label` varchar(100) NOT NULL,
  `model` varchar(100) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `app_label` (`app_label`,`model`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=11 ;

--
-- Contenu de la table `django_content_type`
--

INSERT INTO `django_content_type` (`id`, `name`, `app_label`, `model`) VALUES
(1, 'permission', 'auth', 'permission'),
(2, 'group', 'auth', 'group'),
(3, 'user', 'auth', 'user'),
(4, 'content type', 'contenttypes', 'contenttype'),
(5, 'session', 'sessions', 'session'),
(6, 'site', 'sites', 'site'),
(7, 'log entry', 'admin', 'logentry'),
(8, 'amino acid', 'website', 'aa'),
(9, 'impossible cut', 'website', 'impossiblecut'),
(10, 'enzyme', 'website', 'enzyme');

-- --------------------------------------------------------

--
-- Structure de la table `django_session`
--

CREATE TABLE IF NOT EXISTS `django_session` (
  `session_key` varchar(40) NOT NULL,
  `session_data` longtext NOT NULL,
  `expire_date` datetime NOT NULL,
  PRIMARY KEY (`session_key`),
  KEY `django_session_c25c2c28` (`expire_date`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Contenu de la table `django_session`
--

INSERT INTO `django_session` (`session_key`, `session_data`, `expire_date`) VALUES
('0ebc56ae4b2931351accbd081a19e34d', 'MDAyN2FiYTA1OGRjZGUyMGRiYTA1ZTIwZTJhNGE5Y2JlYTdmMjUxZDqAAn1xAShVEl9hdXRoX3Vz\nZXJfYmFja2VuZHECVSlkamFuZ28uY29udHJpYi5hdXRoLmJhY2tlbmRzLk1vZGVsQmFja2VuZHED\nVQ1fYXV0aF91c2VyX2lkcQSKAQF1Lg==\n', '2013-03-27 19:16:44'),
('938c9e094cc3dbb8df7d94571e832778', 'MDAyN2FiYTA1OGRjZGUyMGRiYTA1ZTIwZTJhNGE5Y2JlYTdmMjUxZDqAAn1xAShVEl9hdXRoX3Vz\nZXJfYmFja2VuZHECVSlkamFuZ28uY29udHJpYi5hdXRoLmJhY2tlbmRzLk1vZGVsQmFja2VuZHED\nVQ1fYXV0aF91c2VyX2lkcQSKAQF1Lg==\n', '2013-03-06 15:19:23');

-- --------------------------------------------------------

--
-- Structure de la table `django_site`
--

CREATE TABLE IF NOT EXISTS `django_site` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `domain` varchar(100) NOT NULL,
  `name` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Contenu de la table `django_site`
--

INSERT INTO `django_site` (`id`, `domain`, `name`) VALUES
(1, 'example.com', 'example.com');

-- --------------------------------------------------------

--
-- Structure de la table `website_aa`
--

CREATE TABLE IF NOT EXISTS `website_aa` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `slug` varchar(50) NOT NULL,
  `mass` decimal(15,10) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `website_aa_a951d5d6` (`slug`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=20 ;

--
-- Contenu de la table `website_aa`
--

INSERT INTO `website_aa` (`id`, `name`, `slug`, `mass`) VALUES
(1, 'Tryptophan', 'W', 186.0793130000),
(2, 'Tyrosine', 'Y', 163.0633200000),
(3, 'Arginine', 'R', 156.1011110000),
(4, 'Phenyalanine', 'F', 147.0684140000),
(5, 'Histidine', 'H', 137.0589120000),
(6, 'Methionine', 'M', 131.0404850000),
(7, 'Glutamic Acid', 'E', 129.0425930000),
(8, 'Glutanine', 'Q', 128.0585780000),
(9, 'Lysine', 'K', 128.0949630000),
(10, 'Aspartic Acid', 'D', 115.0269430000),
(11, 'Isoleucine Leucine', 'I_L', 113.0840640000),
(12, 'Cysteine', 'C', 103.0091850000),
(13, 'Threonione', 'T', 101.0476790000),
(14, 'Valine', 'V', 99.0684140000),
(15, 'Proline', 'P', 97.0527640000),
(16, 'Serine', 'S', 87.0320280000),
(17, 'Alanine', 'A', 71.0371100000),
(18, 'Glycine', 'G', 57.0214640000),
(19, 'Asparagine', 'N', 114.0429270000);

-- --------------------------------------------------------

--
-- Structure de la table `website_enzyme`
--

CREATE TABLE IF NOT EXISTS `website_enzyme` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Contenu de la table `website_enzyme`
--

INSERT INTO `website_enzyme` (`id`, `name`) VALUES
(1, 'Trypsine');

-- --------------------------------------------------------

--
-- Structure de la table `website_enzyme_cut_after`
--

CREATE TABLE IF NOT EXISTS `website_enzyme_cut_after` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `enzyme_id` int(11) NOT NULL,
  `aa_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `enzyme_id` (`enzyme_id`,`aa_id`),
  KEY `website_enzyme_cut_after_604035da` (`enzyme_id`),
  KEY `website_enzyme_cut_after_c50c89b8` (`aa_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;

--
-- Contenu de la table `website_enzyme_cut_after`
--

INSERT INTO `website_enzyme_cut_after` (`id`, `enzyme_id`, `aa_id`) VALUES
(2, 1, 3),
(1, 1, 9);

-- --------------------------------------------------------

--
-- Structure de la table `website_enzyme_cut_before`
--

CREATE TABLE IF NOT EXISTS `website_enzyme_cut_before` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `enzyme_id` int(11) NOT NULL,
  `aa_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `enzyme_id` (`enzyme_id`,`aa_id`),
  KEY `website_enzyme_cut_before_604035da` (`enzyme_id`),
  KEY `website_enzyme_cut_before_c50c89b8` (`aa_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Structure de la table `website_enzyme_cut_imposible`
--

CREATE TABLE IF NOT EXISTS `website_enzyme_cut_imposible` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `enzyme_id` int(11) NOT NULL,
  `impossiblecut_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `enzyme_id` (`enzyme_id`,`impossiblecut_id`),
  KEY `website_enzyme_cut_imposible_604035da` (`enzyme_id`),
  KEY `website_enzyme_cut_imposible_5c05ab90` (`impossiblecut_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Contenu de la table `website_enzyme_cut_imposible`
--

INSERT INTO `website_enzyme_cut_imposible` (`id`, `enzyme_id`, `impossiblecut_id`) VALUES
(1, 1, 1);

-- --------------------------------------------------------

--
-- Structure de la table `website_impossiblecut`
--

CREATE TABLE IF NOT EXISTS `website_impossiblecut` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `first_id` int(11) NOT NULL,
  `second_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `website_impossiblecut_6de00ea9` (`first_id`),
  KEY `website_impossiblecut_bd40717a` (`second_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

--
-- Contenu de la table `website_impossiblecut`
--

INSERT INTO `website_impossiblecut` (`id`, `first_id`, `second_id`) VALUES
(1, 9, 15);

--
-- Contraintes pour les tables exportées
--

--
-- Contraintes pour la table `auth_group_permissions`
--
ALTER TABLE `auth_group_permissions`
  ADD CONSTRAINT `group_id_refs_id_3cea63fe` FOREIGN KEY (`group_id`) REFERENCES `auth_group` (`id`),
  ADD CONSTRAINT `permission_id_refs_id_a7792de1` FOREIGN KEY (`permission_id`) REFERENCES `auth_permission` (`id`);

--
-- Contraintes pour la table `auth_permission`
--
ALTER TABLE `auth_permission`
  ADD CONSTRAINT `content_type_id_refs_id_728de91f` FOREIGN KEY (`content_type_id`) REFERENCES `django_content_type` (`id`);

--
-- Contraintes pour la table `auth_user_groups`
--
ALTER TABLE `auth_user_groups`
  ADD CONSTRAINT `group_id_refs_id_f0ee9890` FOREIGN KEY (`group_id`) REFERENCES `auth_group` (`id`),
  ADD CONSTRAINT `user_id_refs_id_831107f1` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`);

--
-- Contraintes pour la table `auth_user_user_permissions`
--
ALTER TABLE `auth_user_user_permissions`
  ADD CONSTRAINT `permission_id_refs_id_67e79cb` FOREIGN KEY (`permission_id`) REFERENCES `auth_permission` (`id`),
  ADD CONSTRAINT `user_id_refs_id_f2045483` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`);

--
-- Contraintes pour la table `django_admin_log`
--
ALTER TABLE `django_admin_log`
  ADD CONSTRAINT `content_type_id_refs_id_288599e6` FOREIGN KEY (`content_type_id`) REFERENCES `django_content_type` (`id`),
  ADD CONSTRAINT `user_id_refs_id_c8665aa` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`);

--
-- Contraintes pour la table `website_enzyme_cut_after`
--
ALTER TABLE `website_enzyme_cut_after`
  ADD CONSTRAINT `aa_id_refs_id_c07f3265` FOREIGN KEY (`aa_id`) REFERENCES `website_aa` (`id`),
  ADD CONSTRAINT `enzyme_id_refs_id_82dfd63f` FOREIGN KEY (`enzyme_id`) REFERENCES `website_enzyme` (`id`);

--
-- Contraintes pour la table `website_enzyme_cut_before`
--
ALTER TABLE `website_enzyme_cut_before`
  ADD CONSTRAINT `aa_id_refs_id_248f1ef5` FOREIGN KEY (`aa_id`) REFERENCES `website_aa` (`id`),
  ADD CONSTRAINT `enzyme_id_refs_id_60b95b99` FOREIGN KEY (`enzyme_id`) REFERENCES `website_enzyme` (`id`);

--
-- Contraintes pour la table `website_enzyme_cut_imposible`
--
ALTER TABLE `website_enzyme_cut_imposible`
  ADD CONSTRAINT `enzyme_id_refs_id_f91be8cd` FOREIGN KEY (`enzyme_id`) REFERENCES `website_enzyme` (`id`),
  ADD CONSTRAINT `impossiblecut_id_refs_id_f7cf298b` FOREIGN KEY (`impossiblecut_id`) REFERENCES `website_impossiblecut` (`id`);

--
-- Contraintes pour la table `website_impossiblecut`
--
ALTER TABLE `website_impossiblecut`
  ADD CONSTRAINT `first_id_refs_id_f6a4782b` FOREIGN KEY (`first_id`) REFERENCES `website_aa` (`id`),
  ADD CONSTRAINT `second_id_refs_id_f6a4782b` FOREIGN KEY (`second_id`) REFERENCES `website_aa` (`id`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
