var config = {};

config.schemas = {};

config.port = 3000;

config.ifluxEvents = {};
config.ifluxEvents.host = "86.119.25.192";
config.ifluxEvents.path = "/iflux/events";

config.schemas.baseURL = "http://86.119.29.77/iflux";
config.schemas.actionTypePath = "/schemas/actionTypes/arduinoNotification";
config.schemas.eventTypePath = "/schemas/eventTypes/arduinoCommit";

config.githubProjectName = "agile-iflux-simplest";

config.githubApi = {};
config.githubApi.username = "polchky";
config.githubApi.email = "polchky@gmail.com";

module.exports = config;
