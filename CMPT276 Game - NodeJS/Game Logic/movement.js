// Responds to a movement event
socket.on('movement', function(data) {
    
    if (getRoomBySocketId[socket.id] == undefined) {
        return;
    }
    var player = rooms[getRoomBySocketId[socket.id]].players[socket.id] || {};
    movePlayer(player, data, getRoomBySocketId[socket.id]);
});

//Move enemies towards the nearest player
function moveEnemies(rm) {
    //Enemy movement handler
    for (var id in rooms[rm].enemies) {
        //Find closest players
        if ( rooms[rm].players.numPlayers > 0 ) {
            var closestPlayer;
            var closestPlayerDistance = Infinity;
            for (var player in rooms[rm].players) {
                var distX = rooms[rm].players[player].x - rooms[rm].enemies[id].x;
                var distY = rooms[rm].players[player].y - rooms[rm].enemies[id].y;
                var distance = Math.sqrt( distX * distX + distY * distY );
                if (distance < closestPlayerDistance) {
                  closestPlayer = player;
                  closestPlayerDistance = distance;
                }
            }
            if (rooms[rm].players[closestPlayer] == undefined) return;
            
            //Check if close enough to a player to damage them
            distX = rooms[rm].enemies[id].x - rooms[rm].players[closestPlayer].x;
            distY = rooms[rm].enemies[id].y - rooms[rm].players[closestPlayer].y;
            var attackTheta = Math.atan(distX / distY);

            var sign = -1;
            if (rooms[rm].enemies[id].y < rooms[rm].players[closestPlayer].y) {
              sign = 1;
            }
    
            if ( Math.abs(distX) < 15 && Math.abs(distY) < 15 ) {
                //Attack player
                rooms[rm].players[closestPlayer].health -= 8/updatePerSecond;
                //Kill player if they are out of health
                if (rooms[rm].players[closestPlayer].health < 0) {
                    youveBeenTerminated(closestPlayer, rm);
                    if (rooms[rm] == undefined) {
                    return;
                    }
                }
                //Dont move any closer
                sign = 0;
            }
    
            //Move the enemy
            rooms[rm].enemies[id].vx =  rooms[rm].enemies[id].speed * Math.sin(attackTheta) * sign;
            rooms[rm].enemies[id].vy =  rooms[rm].enemies[id].speed * Math.cos(attackTheta) * sign;
            var originX = rooms[rm].enemies[id].x;
            var originY = rooms[rm].enemies[id].y;
            rooms[rm].enemies[id].x += rooms[rm].enemies[id].vx/updatePerSecond;
            rooms[rm].enemies[id].y += rooms[rm].enemies[id].vy/updatePerSecond;

            //Handle enemy-object collision
            if(hasCollision(rooms[rm].enemies[id].x, rooms[rm].enemies[id].y, rm)){
                rooms[rm].enemies[id].x = originX;
                rooms[rm].enemies[id].y = originY;
            }
        }
    }
}
