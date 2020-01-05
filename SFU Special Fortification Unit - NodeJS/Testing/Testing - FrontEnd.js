xmodule.exports = {
  sayHello: function(){
    return 'hello';
  },

  addNumbers: function(value1, value2) {
    return value1 + value2;
  },
  //Tests that roomData() creates a room object
  roomData: function(value1) {
    return roomData(value1);
  },
  //Tests that createRoom() creates a room with name serverName
  createRoom: function(serverName) {
    createRoom(serverName);
    return returnRooms();
  },
  //Tests that createRoom() correctly handles a request to make duplicate room
  createTwoRooms: function(serverName) {
    createRoom(serverName);
    createRoom(serverName);
    return returnRooms();
  },
  //Tests that createlayer() correctly creates a player with ID socketID inside
  //the room serverName
  createPlayer: function(socketID, serverName, username) {
    createRoom(serverName);                        //Create a room
    createPlayer(socketID, serverName, username);  //Create player within room
    return returnRooms();
  },
  //Test player movement
  testMovePlayer: function(socketID, serverName, username, directionData) {
    createPlayer(socketID, serverName, username);  //Create a player for moving
    player = rooms[serverName].players[socketID];
    origin = [player.x, player.y];                 //Player's starting position

    movePlayer(player, directionData, serverName); //Move the player
    result = [player.x, player.y];                 //Position player moved to
    return { "start" : origin, "end" : result, "speed" : player.speed };
  },

  //Test player-wall collisions
  testCollision: function(socketID, serverName, username, directionData) {
    createRoom(serverName);
    createPlayer(socketID, serverName, username);
    player = rooms[serverName].players[socketID]
    for (i = 0; i < 200; i++) {
      movePlayer(player, directionData, serverName);
    }
    ddx = 0; ddy = 0;
    if (directionData.left) {
      ddx -= player.speed/updatePerSecond;
    }
    if (directionData.right) {
        ddx += player.speed/updatePerSecond;
    }
    if (directionData.down) {
        ddy += player.speed/updatePerSecond;
    }
    if (directionData.up) {
        ddy -= player.speed/updatePerSecond;
    }
    return hasCollision((player.x + ddx), (player.y + ddy), serverName);
  },

  //Projectile testing
  generateProjectiles: function(socketID, rm, msCoords) {
    createRoom(rm);                             //Create a gun range
    createPlayer(socketID, rm, "OJ");           //Create a shooter
    generateProjectile(socketID, msCoords, rm); //Create projectile
    return returnProjectiles(rm);
  },
  moveProjectiles: function(rm) {
    moveProjectiles(rm);
    return returnProjectiles(rm);
  },
  deleteProjectile: function(projectileID, rm) {
    deleteBullet(projectileID, rm);
    return returnProjectiles(rm);
  },
  // Tests randomObjects aka enemies spawn correctlty
  testSpawn: function(rm) {
    createRoom(rm);
    var mapDataFromFile = JSON.parse(fs.readFileSync(
      'static/objects/testMap2.json', 'utf8'));
    var processor = require('./static/objects/mapProcessor.js');
    rooms[rm].mapData = processor.constructFromData(mapDataFromFile);
    rooms[rm].zones = processor.constructZone(mapDataFromFile);
    spawnEnemies(rm);

    var enemySuccessfullySpawns = 0
    console.log("logging the room's enemies", rooms[rm].enemies);
    if(rooms[rm].enemies) {
      enemySuccessfullySpawns = 1
    }
    return enemySuccessfullySpawns;
  },
  
  // Test that the boss spawns
  testBossSpawn: function(rm) {
    createRoom(rm);
    releaseTheBeast(rm);
    return(rooms[rm].boss);
  },

  // Test spawn zones
  testZones: function(socketID, rm) {
    createRoom(rm);
    //Load map data
    var mapDataFromFile = JSON.parse(fs.readFileSync(
      'static/objects/testMap2.json', 'utf8'));
    var processor = require('./static/objects/mapProcessor.js');
    rooms[rm].mapData = processor.constructFromData(mapDataFromFile);
    rooms[rm].zones = processor.constructZone(mapDataFromFile);

    createPlayer(socketID, rm, "Room");

    //Each of the below coordinates marks a zone on the map
    zoneList = [[1980, 1555], [2600, 1470], [3500,1400], [2675, 1260],
    [4005, 2460], [2600, 2330], [1955, 2045], [1765, 1740], [1075, 1655]];
    resultList = [];

    for (id in zoneList) {
      zone = zoneList[id];
      rooms[rm].players[socketID].x = zone[0];
      rooms[rm].players[socketID].y = zone[1];

      player = rooms[rm].players[socketID];
      for (zoneNum in rooms[rm].zones) {
        if (rooms[rm].zones[zoneNum].inside(player.x/10, player.y/10)) {
          rooms[rm].players[socketID].zone = zoneNum;
          resultList.push(rooms[rm].players[socketID].zone);
        }
      }
    }
    return resultList;
  },

  duplicateRooms: function(rm) {
    createRoom(rm);
    createRoom(rm);
    //Will crash if unsuccessfull
    return 1;
  },

  nearDeathCheck: function() {
    createRoom("nearDeathRoom");
    createPlayer("nearDeathID", "nearDeathRoom", "uname");
    youveBeenTerminated("nearDeathID",  "nearDeathRoom");
    return(rooms["nearDeathRoom"].players["nearDeathID"]);
  }
}