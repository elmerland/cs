<?php
  include "../util.php";
  $uid = $_GET["uid"];
  $q = "select uid, first_name, last_name, gender, age, location from userinfo where uid = '".$uid."'";
  $result = get_query($q, false);

  if (!$result) {
    echo("Query was not successful");
    die;
  }

  $row = pg_fetch_row($result);

  $first_name = $row[1];
  $last_name = $row[2];

  pg_result_seek($result,0);
?>
<!DOCTYPE html>
<html>
  <head>
    <title>Modern Shadow</title>
    <link href='http://fonts.googleapis.com/css?family=Source+Sans+Pro:600|Noto+Sans|Arimo|Josefin+Sans:700|Oxygen:700|Fjord+One|Dorsa' rel='stylesheet' type='text/css'>
    <link rel="stylesheet" href="http://yui.yahooapis.com/pure/0.4.2/base-min.css">
    <link rel="stylesheet" href="http://yui.yahooapis.com/pure/0.4.2/pure-min.css">
    <link rel="stylesheet" href="user.css">

    <script language="javascript" type="text/javascript" src="http://code.jquery.com/jquery-2.1.0.min.js"></script>
  </head>
  <body>

    <div class="pure-g">
      <header class="sidebar pure-u pure-u-1-4">
        <div>
          <hgroup>
            <h1>MODERN SHADOW</h1>
            <h2><span>user:</span> <?php echo $first_name." ".$last_name  ?></h2>
          </hgroup>
        
          <nav>
            <form action="../friend_requests/send_request.html" method="GET">
              <input type=hidden name="uid" value=<?php echo "\"".$uid."\"" ;?> />
              <input type=submit value="Send Friend Request" class="pure-button" />
            </form>

            <form action="../friend_requests/friend_requests.html" method="GET">
              <input type=hidden name="uid" value=<?php echo "\"".$uid."\"";?> />
              <input type=submit value="View friend requests" class="pure-button"/>
            </form>

            <form action="../ratings/rating.html" method="GET">
              <input type=hidden name="uid" value=<?php echo "\"".$uid."\"";?> />
              <input type=submit value="Rate a Video" class="pure-button"/>
            </form>

            <form action="../index.php" method="GET">
              <input type=submit value="Go To Home Page" class="pure-button"/>
            </form>
          </nav>
        </div>
      </header>



      <article class="content pure-u pure-u-3-4">
        <div class="user-info">
          <section>
            <h3><span class="brackets left">[</span>ABOUT ME<span class="brackets right">]</span></h3>
            <div class="center-outer"><div class="center-middle"><div class="center-inner">
            <?php
              print_user_info($result);
            ?>
            </div></div></div>
          </section>
        </div>
        <div class="pure-g">
          <div class="pure-u pure-u-1-2">
            <section>
              <h3><span class="brackets left">[</span>MY FRIENDS<span class="brackets right">]</span></h3>
              <div class="center-outer"><div class="center-middle"><div class="center-inner">
              <?php
                // $q = "select * from friends where uid2 = ".$uid." or uid2 = ".$uid.";";
                $q = "select uid, first_name, last_name from userinfo, friends where friends.uid1 = ".$uid." and userinfo.uid = friends.uid2";
                $result = get_query($q, false);

                if (!$result) {
                  echo "<p class=\"message\">How sad, no friends :(</p>";
                } else {
                  $headers = "<thead><tr><td>UID</td><td>First Name</td><td>Last Name</td></tr></thead>";
                  print_pure_table($result, $headers);
                }
              ?>
              </div></div></div>
          </section>
          </div>
          <div class="pure-u pure-u-1-2">
            <section>
              <h3><span class="brackets left">[</span>MY RATINGS<span class="brackets right">]</span></h3>
              <div class="center-outer"><div class="center-middle"><div class="center-inner">
              <?php
                $q = "select videoinfo.vid as Video_ID, title, rate_score as Rating from ratings, videoinfo where uid = '".$uid."' and videoinfo.vid = ratings.vid";
                $result = get_query($q, false);

                if (!$result) {
                  echo "<p class=\"message\">Bro, do you even watch movies? No ratings... :\\</p>";
                } else {
                  $headers = "<thead><tr><td>VID</td><td>Title</td><td>Rating</td></tr></thead>";
                  print_pure_table($result, $headers);
                }
              ?>
              </div></div></div>
          </section>
          </div>
        </div>
      </article>
    </div>
</body>
</html>