delimiter $$

USE `imdb`;

CREATE TABLE `my_movies` (
  `id` int(11) NOT NULL,
  `path` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1$$

CREATE  OR REPLACE VIEW `imdb`.`new_view` AS
select b.id, b.year, b.name, c.genre, e.last_name, a.path
from my_movies a
inner join movies b on a.id = b.id
inner join movies_genres c on b.id = c.movie_id
inner join movies_directors d on b.id = d.movie_id
inner join directors e on d.director_id = e.id
;
