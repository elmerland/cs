<!DOCTYPE html>
<html>
  <head>
  <title>Modern Shadow</title>
  <link href='http://fonts.googleapis.com/css?family=Source+Sans+Pro:600|Noto+Sans|Arimo|Josefin+Sans:100|Oxygen:700|Fjord+One' rel='stylesheet' type='text/css'>
  <link href='http://fonts.googleapis.com/css?family=Source+Sans+Pro:600|Noto+Sans|Arimo|Josefin+Sans:100|Oxygen:700|Fjord+One|Dorsa' rel='stylesheet' type='text/css'>
  <link rel="stylesheet" type="text/css" href="normalize.css">
  <link rel="stylesheet" type="text/css" href="table.css">
  <link rel="shortcut icon" href="favicon.ico">
  </head>
  <body>
  <header>
    <h1 class="reflected">Modern Shadow</h1>
  </header>

  <article>
    <h2>
    <span class="brackets left">[</span>
      <?php echo $_POST["title"]; ?>
    <span class="brackets right">]</span>
    </h2>

    <section>
    <div class="outer"><div class="middle"><div class="inner">
      <?php
        include "util.php";

        // Get paramters
        $query = $_POST["query"];
        $verbose = $_POST["verbose"];

        // Get verbose value
        if ($verbose == "true") {
          $verbose = true;
        } else if ($verbose == "false") {
          $verbose = false;
        }

        // Send query
        $result = get_query($query, $verbose);
        if ($result) {
          // Print results
          print_table($result);
          // Print sucess statement.
          echo '<p class="message positive">Query successfully executed!</p>';
        }
        
      ?>
    </div></div></div>
    </section>

    <footer>
    <section>
      <div class="big_bracket left">{</div>
      <div>
        <p>
          CS 4604 - Project<br>
          Professor:
        </p>
        <p class="right">Dr. B. Aditya Prakash</p>
        <p>Team Members:</p>
        <p class="right">
          Samantha Fisher<br>
          Elmer Landaverde<br>
          Taylor Sims
        </p>
      </div>
      <div class="big_bracket right">}</div>
    </section>
    </footer>
  </article>
  </body>
</html>
