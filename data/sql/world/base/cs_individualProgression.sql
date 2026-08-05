DELETE FROM `command` WHERE `name` IN ('ip get', 'ip set', 'ip setbot', 'ip setrep', 'ip tele', 'ip pet', 'ip pvp', 'ip attune');
INSERT INTO `command` (`name`, `security`, `help`) VALUES
('ip get', 0, 'Syntax: .ip get [$player]\nShows the current progression level for yourself, your target, or a named player.'),
('ip set', 2, 'Syntax: .ip set $progressionLevel\nSets the player to the given progression level.'),
('ip get', 0, 'Syntax: .ip get\nShows your or your targets current progression level.'),
('ip tele', 2, 'Syntax: .ip tele $location\nTeleports the player to the given location.');
