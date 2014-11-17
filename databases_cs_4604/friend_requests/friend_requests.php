<?php
  include "../util.php";
  $function = $_GET['function'];
  if ($function == "get_friends_requests") {
    $uid = $_GET['uid'];
    return get_friends_requests($uid);
  } else if ($function == "accept_friend") {
    $uid1 = $_GET['uid1'];
    $uid2 = $_GET['uid2'];
    return accept_friend($uid1, $uid2);
  } else if ($function == "decline_friend") {
    $uid1 = $_GET['uid1'];
    $uid2 = $_GET['uid2'];
    return decline_friend($uid1, $uid2);
  } else if ($function == "send_friend_request") {
    $uid1 = $_GET['uid1'];
    $uid2 = $_GET['uid2'];
    return send_friend_request($uid1, $uid2);
  }

  function get_friends_requests($uid) {
    $q = "select uid1, uid2, first_name, last_name, age, gender, location, requesttime ";
    $q .= "from friendrequests, userinfo ";
    $q .= "where uid2 = '".$uid."' and friendrequests.uid1 = userinfo.uid and response IS NULL ";
    $q .= "ORDER BY last_name ASC;";
    // $q .= "where uid2 = '".$uid."' and friendrequests.uid1 = userinfo.uid";
    $result = get_query($q, false);

    if (!$result) {
      echo "<p class=\"no_requests\">You have no friend requests</p>";
    } else {
      print_friend_request_pure_table($result);
    }
  }

  function accept_friend($uid1, $uid2) {
    $query = "update friendrequests set responsetime = current_time, response = 'Y' where uid1 = ".$uid1." and uid2 = ".$uid2.";";
    $query .= "insert into friends values (".$uid1.", ".$uid2.");";
    $query .= "insert into friends values (".$uid2.", ".$uid1.");";

    $result = get_query($query, false);

    if (!$result) {
      echo "<tr><td>".$uid1."</td><td>Could not accept friend</td></tr>";
    } else {
      echo "<tr><td>".$uid1."</td><td>Friend request accepted!</td></tr>";
    }
  }

  function decline_friend($uid1, $uid2) {
    $query = "update friendrequests set responsetime = current_time, response = 'N' where uid1 = ".$uid1." and uid2 = ".$uid2.";";

    $result = get_query($query, false);

    if (!$result) {
      echo "<tr><td>".$uid1."</td><td>Coudl not decline friend</td></tr>";
    } else {
      echo "<tr><td>".$uid1."</td><td>Friend request declined</tr></td>";
    }
  }

  function send_friend_request($uid1, $uid2) {
    // Check if they are already friends.
    $q = "select * from friends where uid1 = ".$uid1." and uid2 = ".$uid2.";";
    $result = get_query($q, false);

    if ($result) {
      echo "You are already friends!";
      return;
    } 

    // Check if he/she alreayd sent a friend request.
    $q = "select * from friendrequests where uid1 = ".$uid1." and uid2=".$uid2." and response is null;";
    $result = get_query($q, false);

    if ($result) {
      echo "<tr><td>".$uid2."</td><td>You already sent a friend request</td></tr>";
    } else {
      $q = "insert into friendrequests values (".$uid1.", ".$uid2.", current_time);";
      // $q = "insert into friendrequests values (27, 27810, current_time);"
      $result = get_query($q, false);
      if (!$result) {
        echo "<tr><td>".$uid2."</td><td>Could not send friend request</td></tr>";
      } else {
        echo "<tr><td>".$uid2."</td><td>Friend request sent</td></tr>";
      }
    }
  }
?>
