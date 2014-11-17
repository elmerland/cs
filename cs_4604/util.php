<?php
  /**
   * Takes a query result and formats it into a table.
   * @param  [type] $query_result The results form a query.
   * @return [type]               The results of the query formatted in a table.
   */
  function print_table($query_result) {
    echo "<table>";
    echo "<tr>";
    for ($i = 0; $i < pg_num_fields($query_result); $i++) {
      $fieldname = pg_field_name($query_result, $i);
      echo "<th>$fieldname</th>";
    }
    echo "</tr>";

    while ($row = pg_fetch_row($query_result)) {
      echo "<tr>";
      $num = pg_num_fields($query_result);
      for ($i = 0; $i < $num; $i++) {
        echo "<td>$row[$i]</td>";
      }
      echo "</tr>";
    }
    echo "</table>";
  }

  /**
   * Takes a query result and formats it into a table.
   * @param  [type] $query_result The results form a query.
   * @return [type]               The results of the query formatted in a table.
   */
  function print_pure_table($query_result, $headers) {
    echo "<table class=\"pure-table pure-table-bordered\">";
    if ($headers) {
      echo $headers;
    } else {
      echo "<thead><tr>";
      for ($i = 0; $i < pg_num_fields($query_result); $i++) {
        $fieldname = pg_field_name($query_result, $i);
        echo "<th>$fieldname</th>";
      }
      echo "</tr></thead>";
    }
    
    echo "<tbody>";
    while ($row = pg_fetch_row($query_result)) {
      echo "<tr>";
      $num = pg_num_fields($query_result);
      for ($i = 0; $i < $num; $i++) {
        echo "<td>$row[$i]</td>";
      }
      echo "</tr>";
    }
    echo "</tbody>";
    echo "</table>";
  }

  /**
   * Takes a query result and formats it into a table.
   * @param  [type] $query_result The results form a query.
   * @return [type]               The results of the query formatted in a table.
   */
  function print_friend_request_pure_table($query_result) {
    echo "<table class=\"pure-table pure-table-bordered\">";

    echo "<thead><tr>";
    echo "<td>UID</td>";
    echo "<td>UID2</td>";
    echo "<td>First Name</td>";
    echo "<td>Last Name</td>";
    echo "<td>Age</td>";
    echo "<td>Gender</td>";
    echo "<td>Location</td>";
    echo "<td>Request Time</td>";
    echo "</tr></thead>";

    // echo "<thead><tr>";
    // for ($i = 0; $i < pg_num_fields($query_result); $i++) {
    //   $fieldname = pg_field_name($query_result, $i);
    //   echo "<th>$fieldname</th>";
    // }
    // echo "</tr></thead>";
    echo "<tbody>";
    while ($row = pg_fetch_row($query_result)) {
      echo "<tr>";
      $num = pg_num_fields($query_result);
      for ($i = 0; $i < $num; $i++) {
        echo "<td>$row[$i]</td>";
      }
      echo "</tr>";
    }
    echo "</tbody>";
    echo "</table>";
  }

  /**
   * Takes a query result and formats it into a table.
   * @param  [type] $query_result The results form a query.
   * @return [type]               The results of the query formatted in a table.
   */
  function print_user_info($query_result) {

    $row = pg_fetch_row($query_result);

    echo '<div class="user-info pure-g">';
    echo '<div class="pure-u pure-u-1-6">';
    echo '<dl><dt>UID</dt><dd>'.$row[0].'</dd></dl>';
    echo '</div>';
    echo '<div class="pure-u pure-u-1-6">';
    echo '<dl><dt>First Name</dt><dd>'.$row[1].'</dd></dl>';
    echo '</div>';
    echo '<div class="pure-u pure-u-1-6">';
    echo '<dl><dt>Last Name</dt><dd>'.$row[2].'</dd></dl>';
    echo '</div>';
    echo '<div class="pure-u pure-u-1-6">';
    echo '<dl><dt> Gender</dt><dd>'.$row[3].'</dd></dl>';
    echo '</div>';
    echo '<div class="pure-u pure-u-1-6">';
    echo '<dl><dt>Age</dt><dd>'.$row[4].'</dd></dl>';
    echo '</div>';
    echo '<div class="pure-u pure-u-1-6">';
    echo '<dl class="last-child"><dt>Location</dt><dd>'.$row[5].'</dd></dl>';
    echo '</div>';
    echo '</div>';
  }

  /**
   * Display the results of a user name query.
   * @param  number $display_count The limit placed on the query.
   * @param  resource $count         The number of all name matches.
   * @param  resource $query_result  The name matches.
   */
  function print_videoinfo_results($display_count, $count, $query_result) {
    // Get the total number of matches
    $count_row = pg_fetch_row($count);
    $total_count = $count_row[0];
    // Update the display count to be the minimum between display count and total count.
    $display_count = $total_count < $display_count ? $total_count : $display_count;    

    // Print results table.
    echo "<table class=\"pop-over-table pure-table pure-table-bordered\">";
    echo "<thead><tr>";
    echo "<td>VID</td>";
    echo "<td>Title</td>";
    echo "<td>Year</td>";
    echo "<td>Certification</td>";
    echo "</tr></thead><tbody>";

    while ($row = pg_fetch_row($query_result)) {
      echo "<tr>";
      $num = pg_num_fields($query_result);

      for ($i = 0; $i < $num; $i++) {
        if ($i == 0) {
          echo '<td class="videoinfo_vid">'.$row[$i].'</td>';
        } else if ($i == 1) {
          echo '<td class="videoinfo_title">'.$row[$i].'</td>';
        } else if ($i == 2) {
          echo '<td class="videoinfo_year">'.$row[$i].'</td>';
        } else if ($i == 3) {
          echo '<td class="videoinfo_certification">'.$row[$i].'</td>';
        } else {
          echo"<td>".$row[$i]."</td>";
        }
      }
      echo "<tr>";
    }
    echo "</tbody></table>";

    // Echo display count summary.
    echo "<p class=\"count_summary\">Showing: ".$display_count." of ".$total_count."</p>";
  }

  /**
   * Display the results of a user name query.
   * @param  number $display_count The limit placed on the query.
   * @param  resource $count         The number of all name matches.
   * @param  resource $query_result  The name matches.
   */
  function print_user_name_results($display_count, $count, $query_result) {
    // Get the total number of matches
    $count_row = pg_fetch_row($count);
    $total_count = $count_row[0];
    // Update the display count to be the minimum between display count and total count.
    $display_count = $total_count < $display_count ? $total_count : $display_count;    

    // Print results table.
    echo "<table class=\"pop-over-table pure-table pure-table-bordered\">";
    echo "<thead><tr>";
    echo "<td>UID</td>";
    echo "<td>First Name</td>";
    echo "<td>Last Name</td>";
    echo "</tr></thead><tbody>";
    while ($row = pg_fetch_row($query_result)) {
      echo "<tr>";
      $num = pg_num_fields($query_result);
      for ($i = 0; $i < $num; $i++) {
        // echo"<td>".$row[$i]."</td>";
        if ($i == 0) {
          echo '<td class="userinfo_uid">'.$row[$i].'</td>';
        } else if ($i == 1) {
          echo '<td class="userinfo_first_name">'.$row[$i].'</td>';
        } else if ($i == 2) {
          echo '<td class="userinfo_last_name">'.$row[$i].'</td>';
        } else {
          echo"<td>".$row[$i]."</td>";
        }
      }
      echo "<tr>";
    }
    echo "</tbody></table>";

    // Echo display count summary.
    echo "<p class=\"count_summary\">Showing: ".$display_count." of ".$total_count."</p>";
  }

  /**
   * Prints the error code if verbose is set to true and returns false.
   * @param  boolean $verbose Set to true if error code should be printed.
   * @param  text $error   The error code.
   * @return boolean          False
   */
  function print_error($verbose, $error) {
    if ($verbose) {
      echo $error;
    }
    return false;
  }

  /**
   * This function sends a query to the data base and returns the result of the
   * query. When the query could not be run or an error occured then 'false' is
   * returned. Otherwise if the query executed successfully a result 'resource'
   * is returned. NOTE: When a query is executed that doesn't return anything,
   * this function returns 'true'. This happens when a command is executed.
   * @param  text $query        The query to be executed
   * @param  boolean $verbose   Set to true to print error messages
   * @return resource | boolesn A resource if the query was successfully
   *                            executed, false if there was an error.
   */
  function get_query($query, $verbose) {
    // return $query.", ".$verbose;
    // Establish connection with data base.
    $conn_str = "port=5432 dbname=Modern\ Shadow user=elmer password=QSCft67ukl";
    $conn = pg_connect($conn_str);

    if (!$conn) {
      $error = '<p class="message negative">Could not connect to database</p>';
      return print_error($verbose, $error);
    }

    // Determine if connection is busy.
    if (pg_connection_busy($conn, $verbose)) {
      pg_close($conn);
      $error = '<p class="message negative">Connection busy</p>';
      return print_error($verbose, $error);
    }

    if (!pg_send_query($conn, $query)) {
      pg_close($conn);
      $error =  '<p class="message negative">Could not send query</p>';
      return print_error($verbose, $error);
    }

    // Get query result
    $result = pg_get_result($conn);

    // Check query result status
    $error_code = pg_result_status($result);
    if ($error_code == PGSQL_BAD_RESPONSE) {
      pg_close($conn);
      $error = '<p class="message negative">Error: The servers response was not understood</p>';
      return print_error($verbose, $error);
    } else if ($error_code == PGSQL_NONFATAL_ERROR) {
      pg_close($conn);
      $error = '<p class="message negative">Error: A nonfatal error (a notice or warning) occurred</p>';
      return print_error($verbose, $error);
    } else if ($error_code == PGSQL_FATAL_ERROR) {
      pg_close($conn);
      $error = '<p class="message negative">Error: Invalid query</p>';
      return print_error($verbose, $error);
    } else if ($error_code == PGSQL_EMPTY_QUERY) {
      pg_close($conn);
      $error = '<p class="message negative">Error: Empty query</p>';
      return print_error($verbose, $error);
    } else if ($error_code == PGSQL_COMMAND_OK) {
      pg_close($conn);
      $message = '<p class="message positive">Command successful, but no data was returned</p>';
      return true;
    } else if ($error_code == PGSQL_COPY_OUT) {
      pg_close($conn);
      $error = '<p class="message neutral">Copy Out (from server) data transfer started</p>';
      return print_error($verbose, $error);
    } else if ($error_code == PGSQL_COPY_IN) {
      pg_close($conn);
      $error = '<p class="message neutral">Copy In (to server) data transfer started</p>';
      return print_error($verbose, $error);
    }

    // Check query length
    if (pg_num_rows($result) == 0) {
      pg_close($conn);
      $error = '<p class="message neutral">Query result is empty</p>';
      return print_error($verbose, $error);
    }

    // Close connection
    pg_close($conn);

    // Return query result
    return $result;
  }
?>