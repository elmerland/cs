
<!DOCTYPE html>
<html>
  <head>
    <title>Modern Shadow</title>
    <link href='http://fonts.googleapis.com/css?family=Source+Sans+Pro:600|Noto+Sans|Arimo|Josefin+Sans:100|Oxygen:700|Fjord+One' rel='stylesheet' type='text/css'>
    <link rel="stylesheet" type="text/css" href="normalize.css">
    <link rel="stylesheet" href="http://yui.yahooapis.com/pure/0.4.2/base-min.css">
    <link rel="stylesheet" href="http://yui.yahooapis.com/pure/0.4.2/pure-min.css">
    <link rel="stylesheet" type="text/css" href="main.css">

    <link rel="shortcut icon" href="favicon.ico">
    <script language="javascript" type="text/javascript" src="http://code.jquery.com/jquery-2.1.0.min.js"></script>
    <script language="javascript" type="text/javascript" src="util.js"></script>
    <script language="javascript" type="text/javascript" src="main.js"></script>
    <script language="javascript" type="text/javascript" src="login.js"></script>
  </head>
  <body>

    <header>
      <h1 class="reflected">Modern Shadow</h1>
    </header>

    <article class="login">
      <section>
        <h2><span class="brackets left">[</span>SELECT USER ACCOUNT<span class="brackets right">]</span></h2>
        <form autocomplete="off">
          <input type=text name="first_name" placeholder="First name..." id="first_name" class="input-field user_name_search"/>
          <input type=text name="last_name" placeholder="Last name..." id="last_name" class="input-field user_name_search" />
          <input type=submit value="login" id="submit_button" class="request-buttons"/>
        </form>
        <div id="pop_over"></div>
      </section>
    </article>

    <article class="team">
      <section>
        <h2><span class="brackets left">[</span>TEAM MEMBERS<span class="brackets right">]</span></h2>
        <p>
          <a href="http://courses.cs.vt.edu/~cs4604/Spring14/">CS 4604 - Project</a><br><br>
          <a href="http://people.cs.vt.edu/~badityap/">Professor: Dr. B. Aditya Prakash</a>
        </p>
        <ul>
          <li>Sam Fisher</li>
          <li><a href="http://learncodeart.com">Elmer Landaverde</a></li>
          <li><a href ="http://russianflu.lib.vt.edu/">Taylor Sims</a></li>
        </ul>
        <div class="clear"></div>
        
      </section>
    </article>

    <article class="relations">
      <section>
        <h2><span class="brackets left">[</span>RELATIONS<span class="brackets right">]</span></h2>
        <dl>
          <dt><a id="rel_userinfo" href="#">USERINFO (uid, age, gender, first_name, last_name, location)</a></dt>
            <dd>This relationship belongs to the User Info table. This table stores all the necessary details to uniquely identify a user along with other attributes that help describe the user.</dd>
          <dt><a id="rel_performer" href="#">PERFORMER (pid, first_name, last_name, age, gender)</a></dt>
            <dd>This relationship belongs to the performer table.  This tbale stores the details that identify a performer, including name, age and gender.</dd>
          <dt><a id="rel_director" href="#">DIRECTOR (did, first_name, last_name, age)</a></dt>
            <dd>This table is used to store the information that identifies a director.</dd>
          <dt><a id="rel_certificates" href="#">CERTIFICATES (certification_name, content)</a></dt>
            <dd>The certificates relationship stores information about the certificate, including name and content.</dd>
          <dt><a id="rel_genre" href="#">GENRE (genre)</a></dt>
            <dd>This table is used to store the different genres a movie can possibly be.</dd>
          <dt><a id="rel_belongtogenre" href="#">BELONGTOGENRE (vid, genre)</a></dt>
            <dd>This table stores a video id and the genre the video belongs to.</dd>
          <dt><a id="rel_videoinfo" href="#">VIDEOINFO (vid, title, release_year, producer, color, country, certification_name)</a></dt>
            <dd>This is the main table of the data base, its job is to keep track of all videos available to rent/purchase in the store.</dd>
          <dt><a id="rel_movieinfo" href="#">MOVIEINFO (mid, mtitle)</a></dt>
            <dd>The movie info table stores the movie title and its unique id number.</dd>
          <dt><a id="rel_ratings" href="#">RATINGS (vid, uid, rate_score, ratingtime)</a></dt>
            <dd>The ratings relationship stores information about each rating a user makes including the video watched, the rating, and the time the rating was made.</dd>
          <dt><a id="rel_friendrequests" href="#">FRIENDREQUESTS (uid1, uid2, requesttime, response, responsetime time)</a></dt>
            <dd>This table is used to keep track of any attempts of one user to friend other users. A user is able to send a friend request several times to any particular user. Two users are only consider friends when the recipient of the friend request accepts the request.</dd>
          <dt><a id="rel_friends" href="#">FRIENDS (uid1, uid2)</a></dt>
            <dd>The friends table stores the user ids of users that are friends with one another.</dd>
          <dt><a id="rel_actin" href="#">ACTIN (vid, pid)</a></dt>
            <dd>The actin table stores information regarding the performer and what video they star in.</dd>
          <dt><a id="rel_tvepisodeinfo" href="#">TVEPISODEINFO (tvid, episodeNum)</a></dt>
            <dd>The tv episode info relationship allows you to uniquely identify a tv episode from a specific season.</dd>
          <dt><a id="rel_collection" href="#">COLLECTION (collection_id, collection_name)</a></dt>
            <dd>The collection table stores information regarding specific collections.</dd>
          <dt><a id="rel_inseason" href="#">INSEASON (tvid, seasontitle, seasonNum)</a></dt>
            <dd>The inseason relationship relates tv series seasons to their given shows.</dd>
          <dt><a id="rel_season" href="#">SEASON (seasontitle, seasonNum)</a></dt>
            <dd></dd>
          <dt><a id="rel_in_collection" href="#">IN_COLLECTION (collectionid, seasontitle, seasonnum)</a></dt>
            <dd>The In Collection table links several seasons into one video item. The purpose of this table is to be able to offer a collection of videos as a single item. Otherwise, every collection would have to keep track of all of the videos in it at the time of purchase and make sure to charge the right amount.</dd>
          <dt><a id="rel_moviedirectors" href="#">MOVIEDIRECTORS (did, vid)</a></dt>
            <dd>The movie directors relationship relates the director to the movie he or she is directing.</dd>
          <dt><a id="rel_tvepisodedirectors" href="#">TVEPISODEDIRECTORS (did, tvid)</a></dr>
            <dd>The TV episode directors relationship relates a director to the TV series he or she is directing.</dd>
        </dl>
      </section>
    </article>

    <article class="queries">
      <section>
        <h2><span class="brackets left">[</span>QUERIES<span class="brackets right">]</span></h2>
        <dl>
          <dt><a id="query_brad" href="#">Query 1:</a> Lists all the movies that Brad Pitt has starred in.</dt>
            <dd>
              SELECT * FROM videoinfo WHERE vid in (SELECT vid FROM actin WHERE pid = (SELECT pid FROM performer WHERE first_name = 'Brad' AND last_name = 'Pitt')) ORDER BY release_year desc LIMIT 10
            </dd>
          <dt><a id="query_movie_rating" href="#">Query 2:</a> List the top 50 best rated movies.</dt>
            <dd>
              SELECT m.mid, m.mtitle, ROUND(AVG(r.rate_score), 2) AS avg_rate, ROUND(COUNT(r.rate_score), 0) AS rate_count, ROUND((AVG(r.rate_score) * COUNT(r.rate_score)) / COUNT(r.rate_score) * COUNT(r.rate_score)/100, 2) AS score_calc FROM movieinfo AS m, videoinfo AS v, ratings AS r WHERE m.mid = v.vid AND m.mid = r.vid GROUP BY m.mid, m.mtitle ORDER BY score_calc DESC LIMIT 50<br><br>
              This query creates a calculated score to order movies by. This score is computed by the following formula:<br>
              <span class="formula" style="padding-left: 75px;">[avg(rating)*count(rating)]/count(rating) * count(rating)/100</span><br>
              This is done so that movies with very few ratings of 5 do not get ahead of movies with a lot of good ratings.
            </dd>
          <dt><a id="query_year_genre" href="#">Query 3:</a> Lists all the Sci-Fi movies made in 2001.</dt>
            <dd>SELECT v.vid, title, release_year, genre  FROM videoinfo AS v, belongtoGenre AS b WHERE v.vid = b.vid AND v.release_year = 2001 AND genre = 'Sci-Fi'</dd>
          <dt><a id="query_popular_user" href="#">Query 4:</a> Lists the top 25 users with the most friends.</dt>
            <dd>
              CREATE VIEW user_total_friends AS SELECT uid1 as uid, COUNT(distinct uid2) AS friend_num FROM friends GROUP BY uid1<br><br>
              SELECT u.uid, u.last_name, u.first_name, f.friend_num FROM user_total_friends as f, userinfo as u WHERE f.uid = u.uid ORDER BY f.friend_num DESC LIMIT 25
            </dd>
          <dt><a id="query_tvunam" href="#">Query 5:</a> Lists all the movies that were produced by TV UNAM.</dt>
            <dd>SELECT title FROM videoinfo WHERE producer = 'TV UNAM'</dd>
        </dl>
      </section>
    </article>

    <article class="adhoc">
      <section>
        <h2><span class="brackets left">[</span>AD-HOC QUERY<span class="brackets right">]</span></h2>
        <form method=post action="query.php" target="_blank">
          <dl>
            <dt>Enter Query Title:</dt>
              <dd><input type=text name="title" value="CUSTOM QUERY" class="input-field"></dd>
            <dt>Enter Query:</dt>
              <dd><textarea rows="4" placeholder="Input your query here..." name="query" class="input-field"></textarea></dd>
          </dl>
          <div class="buttons">
            <input type=hidden name="verbose" value="true">
            <input type=reset value="Clear" id="clear" class="request-buttons">
            <input type=submit value="Run Query" id="run" class="request-buttons">
          </div>
        </form>
      </section>
    </article>
  </body>
</html>
