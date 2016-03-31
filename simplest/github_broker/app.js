var express = require('express');
var http = require('http');
var bodyParser = require('body-parser');
var config = require('./config');

var app = express();

app.use(bodyParser.json());

// Function passing events to Iflux
var notifyIflux = function(commit) {
	var date = new Date();
        var post_options = {
                host: config.ifluxEvents.host,
                path: config.ifluxEvents.path,
                method: 'POST',
                headers: {
                        'Content-Type': 'application/json'
                }
        }
	var post_data = {
		'timestamp' : date.toISOString(),
                'source' : 'kHC66ypIe1XH',
                'type' : config.schemas.baseURL + config.schemas.eventTypePath,
                'properties': {
                        'projectname' : config.githubProjectName
                }

	};
	var post_req = http.request(post_options, function(res){

        });
	post_req.write(JSON.stringify(post_data));
        post_req.end();
	console.log(" Event sent to Iflux");
}

// Default action type definition
app.get(config.schemas.actionTypePath, function(req, res) {
        res.sendStatus(200);
});

// Default event type definition
app.get(config.schemas.eventTypePath, function(req, res) {
        res.sendStatus(200);
});

// ESP module IP declaration
app.post('/esp', function(req, res) {
        console.log("IP of ESP module: " + JSON.stringify(req.body.ip, null, 2));
        res.sendStatus(200);
});

// Testing purposes only, reception of actions from Iflux
app.post('/actions', function(req, res) {
	console.log(JSON.stringify(req.body));
	res.sendStatus(204);
});

// Wekhook from Github
app.post('/webhook', function(req, res) {
        console.log("push event received");
        res.sendStatus(204);
	var commits = req.body.commits;
	if(Array.isArray(commits)){
		commits.forEach(function(commit) {
			notifyIflux(commit);	
			//console.log(commit);
		});
	}
});

app.listen(config.port, function () {
        console.log("Server started on port " + config.port);
});
