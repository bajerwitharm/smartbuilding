var jwt = require('jwt-simple');

var db;
var users;

var auth = {
    login: function (req, res) {
        var username = req.body.username || '';
        var password = req.body.password || '';
        if (username == '' || password == '') {
            res.status(401);
            res.json({
                "status": 401,
                "message": "Invalid credentials"
            });
            return;
        }

        // Fire a query to your DB and check if the credentials are valid
        auth.validate(username, password, function (dbUserObj) {
            if (!dbUserObj) { // If authentication fails, we send a 401 back
                res.status(401);
                res.json({
                    "status": 401,
                    "message": "Invalid credentials"
                });
                return;
            } else {
                res.json(genToken(dbUserObj));
            }
        });
    },

    validate: function (username, password, callback) {
        return db.validateUserPassword(username, password, callback);
    },

    validateUser: function (username, callback) {
        return db.validateUser(username, callback);
    },
}

// private method
function genToken(user) {
    var expires = expiresIn(7); // 7 days
    var token = jwt.encode({
        exp: expires
    }, require('../config/secret')());

    return {
        token: token,
        expires: expires,
        user: user
    };
}

function expiresIn(numDays) {
    var dateObj = new Date();
    return dateObj.setDate(dateObj.getDate() + numDays);
}

module.exports = function (database) {
    db = database;
    return auth;
}
