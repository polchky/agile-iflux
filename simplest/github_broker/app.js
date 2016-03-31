var express = require('express');
var bodyParser = require('body-parser');
var config = require('./config');

var app = express();

app.use(bodyParser.json());

var debug = true;
function log(message){
        if(debug) console.log(message);
}

app.get('/schemas/actionTypes/arduinoNotification', function(req, res) {
        res.sendStatus(200);
});

app.get('/schemas/eventTypes/arduinoCommit', function(req, res) {
        res.sendStatus(200);
});


app.post('/esp', function(req, res) {
        console.log("IP of ESP module: " + JSON.stringify(req.body.ip, null, 2));
        res.sendStatus(200);
});
app.post('/push', function(req, res) {
        log("push event received");
        res.sendStatus(204);
        req.body.commits.forEach(function(item) {
                log(JSON.stringify(item.id));
        });
});
app.get('/', function(req, res){
        res.send("all right");
        console.log("ok");
});
app.get('/test', function(req, res) {
        res.send("It's ok");
});
app.listen(config.port, function () {
        log("Server started on port " + config.port);
});
