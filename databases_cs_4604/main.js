
$(document).ready(init);

function init() {

  var relations = ["USERINFO",
    "PERFORMER",
    "DIRECTOR",
		"CERTIFICATES",
		"GENRE",
		"BELONGTOGENRE",
		"VIDEOINFO",
		"MOVIEINFO",
		"RATINGS",
		"FRIENDREQUESTS",
		"FRIENDS",
		"ACTIN",
		"TVEPISODEINFO",
		"COLLECTION",
		"INSEASON",
		"SEASON",
		"IN_COLLECTION",
		"MOVIEDIRECTORS",
		"TVEPISODEDIRECTORS"];

  for(var i = 0; i < relations.length; i += 1) {
    addRelationQuery(relations[i].toLowerCase(), i + 1);
  }

  submitQuery('query_brad', 'QUERY 1 - BRAD PITT', "select * from videoinfo where vid in (select vid from actin where pid = (select pid from performer where first_name = 'Brad' AND last_name = 'Pitt')) order by release_year desc");

  submitQuery('query_movie_rating', 'QUERY 2 - BEST RATED MOVIE', "SELECT m.mid, m.mtitle, ROUND(AVG(r.rate_score), 2) AS avg_rate, ROUND(COUNT(r.rate_score), 0) AS rate_count, ROUND((AVG(r.rate_score) * COUNT(r.rate_score)) / COUNT(r.rate_score) * COUNT(r.rate_score)/100, 2) AS score_calc FROM movieinfo AS m, videoinfo AS v, ratings AS r WHERE m.mid = v.vid AND m.mid = r.vid GROUP BY m.mid, m.mtitle ORDER BY score_calc DESC LIMIT 50");

  submitQuery('query_year_genre', 'QUERY 3 - YEAR AND GENRE', "SELECT v.vid, title, release_year, genre  FROM videoinfo AS v, belongtoGenre AS b WHERE v.vid = b.vid AND v.release_year = 2001 AND genre = 'Sci-Fi'");

  submitQuery('query_popular_user', 'QUERY 4 - POPULAR USERS', 'SELECT u.uid, u.last_name, u.first_name, f.friend_num FROM user_total_friends as f, userinfo as u WHERE f.uid = u.uid ORDER BY f.friend_num DESC LIMIT 25')

  submitQuery('query_tvunam', 'QUERY 5 - TVUNMAN', "select title from videoinfo where producer = 'TV UNAM'");

}

function addRelationQuery(table_name, count) {
  submitQuery('rel_' + table_name, table_name.toUpperCase() + ' RELATION', 'SELECT * FROM ' + table_name + ' LIMIT 25');
}

function submitQuery(id, title, query) {
  addPost("#" + id, [['title', title], ['query', query]])
}

function addPost(id, key_value) {
  $(id).click(function (e) {
    e.preventDefault();
    key_value.concat([["verbose", "true"]]);
    submit_form("POST", "query.php", "_blank", key_value);
  });
}

// function addPost(id, key_value) {
//   $(id).click(function (e) {
//     e.preventDefault();

//     var form = '<form action="query.php" method="POST" target="_blank">';
//     for (var i = 0; i < key_value.length; i += 1) {
//       form = form + '<input type="hidden" name="' + key_value[i][0] + '" value="' + key_value[i][1] + '">';
//     }
//     form = form + '<input type="hidden" name="verbose" value="true">';
//     form = form + '</form>'

//     $(form).submit();
//   });
// }
