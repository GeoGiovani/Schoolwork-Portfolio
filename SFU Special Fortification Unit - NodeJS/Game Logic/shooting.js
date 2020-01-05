//Code block to respond to shooting
socket.on('shoot', function(data) {
    //If the player shot on the client side, generate a projectile
    if (data.shootBullet) {
      var rm = getRoomBySocketId[socket.id]
      for (id in rooms[rm].players) {
        player = rooms[rm].players[id];
      }
      generateProjectile(socket.id, data, rm);
    }
});

//Move projectiles
function moveProjectiles(rm) {
  for (var id in rooms[rm].projectiles) {
    if (rooms[rm].projectiles[id]) {
      var delBullet = false;
      var originX = rooms[rm].projectiles[id].x;
      var originY = rooms[rm].projectiles[id].y;
      rooms[rm].projectiles[id].x += rooms[rm].projectiles[id].vx/updatePerSecond;
      rooms[rm].projectiles[id].y += rooms[rm].projectiles[id].vy/updatePerSecond;
      if(hasCollision(rooms[rm].projectiles[id].x, rooms[rm].projectiles[id].y, rm)){
        rooms[rm].projectiles[id].x = originX;
        rooms[rm].projectiles[id].y = originY;
        delBullet = true;
        // deleteBullet(id);
      }
      //Delete stale projectiles
      if ((Math.abs(rooms[rm].projectiles[id].x) > 5000) ||
          (Math.abs(rooms[rm].projectiles[id].y) > 5000)) {
          deleteBullet(id, rm);
      }
    }
  }
}

//Delete a stale bullet
function deleteBullet(id, rm) {
    var temp = rooms[rm].projectiles[rooms[rm].bulletCount -= 1];
    rooms[rm].projectiles[rooms[rm].bulletCount] = rooms[rm].projectiles[id];
    rooms[rm].projectiles[id] = temp;
    rooms[rm].projectiles[rooms[rm].bulletCount] = 0;
    rooms[rm].projectiles.numProjectiles -= 1;
}

//Handles bullet collisions
function handleBulletCollisions(rm) {
  //Player-projectile collision handler
  for (var player in rooms[rm].players) {
    for (var id in rooms[rm].projectiles) {
      if (rooms[rm].projectiles[id]) {
        if ((Math.abs(rooms[rm].players[player].x - rooms[rm].projectiles[id].x) < 2) &&
            (Math.abs(rooms[rm].players[player].y - rooms[rm].projectiles[id].y) < 2)) {
            
          rooms[rm].players[player].health -= 1;
          //Kill dead player
          if (rooms[rm].players[player].health < 0) {
            youveBeenTerminated(player, rm);
          }
        }
      }
    }
  }
  //Enemy-projectile collision handler
  for (var enemy in rooms[rm].enemies) {
    for (var id in rooms[rm].projectiles) {
      if (rooms[rm].projectiles[id]) {
        if ((Math.abs(rooms[rm].enemies[enemy].x - rooms[rm].projectiles[id].x) < 12) &&
            (Math.abs(rooms[rm].enemies[enemy].y - rooms[rm].projectiles[id].y) < 12)) {
            
          rooms[rm].enemies[enemy].health -= 1;
          //Kill dead enemies
          if (rooms[rm].enemies[enemy].health < 0) {
            var temp = rooms[rm].enemies[rooms[rm].enemyID -= 1];
            rooms[rm].enemies[rooms[rm].enemyID] = rooms[rm].enemies[enemy];
            rooms[rm].enemies[enemy] = temp;
            rooms[rm].enemies[rooms[rm].enemyID] = 0;
            rooms[rm].enemies.numEnemies -= 1;
          }
        }
      } 
    }
  }
}

//Reload players gun
function reloadGun(player) {
    player.clip = player.clipSize;
}
  
//Kill a player below 0 health
function youveBeenTerminated(player, rm) {
    rooms[rm].players[player] = 0;
    rooms[rm].numPlayers -= 1;
    if (rooms[rm].numPlayers <= 0) {
      delete rooms[rm];
    }
}