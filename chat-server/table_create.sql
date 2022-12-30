DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS friends;
DROP TABLE IF EXISTS messages;

CREATE TABLE users(
    user_id INT,
    username TEXT,
    password TEXT);

CREATE TABLE friends(
    user1_id INT,
    user2_id INT);

CREATE TABLE messages(
    message TEXT,
    user1_id INT,
    user2_id INT,
    timestamp TEXT);


INSERT INTO users VALUES(1, 'dawids21', 'password');
INSERT INTO users VALUES(2, 'myylek', '12345');
INSERT INTO users VALUES(3, 'jan_kow', 'qwerty');

INSERT INTO friends VALUES(1, 2);
INSERT INTO friends VALUES(1, 3);
INSERT INTO friends VALUES(2, 3);

INSERT INTO messages VALUES('Hi', 1, 2, datetime('now'));
INSERT INTO messages VALUES('Hello', 1, 3, datetime('now'));
INSERT INTO messages VALUES('Whats up', 2, 3, datetime('now'));