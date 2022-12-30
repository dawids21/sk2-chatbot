--sqlite3 chat-db.db -init table_create.sql  
--komenda do inicjalizacji pustej bazy danych

DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS friends;
DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS logged_in_users;

CREATE TABLE users(
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT,
    password TEXT);

CREATE TABLE friends(
    user1_id INTEGER,
    user2_id INTEGER);

CREATE TABLE messages(
    message TEXT,
    user_id INTEGER,
    friend_id INTEGER,
    timestamp TEXT);

-- CREATE TABLE loogged_in_users(
--     user_id INTEGER,
--     token TEXT);





INSERT INTO users(username, password) VALUES('dawids21', 'password');
INSERT INTO users(username, password) VALUES('myylek', '12345');
INSERT INTO users(username, password) VALUES('jan_kow', 'qwerty');

INSERT INTO friends VALUES(1, 2);
INSERT INTO friends VALUES(2, 1);
INSERT INTO friends VALUES(1, 3);
INSERT INTO friends VALUES(3, 1);
INSERT INTO friends VALUES(2, 3);
INSERT INTO friends VALUES(3, 2);

INSERT INTO messages VALUES('Hi', 1, 2, datetime('now', 'localtime'));
INSERT INTO messages VALUES('Hi2', 2, 1, datetime('now', 'localtime'));
INSERT INTO messages VALUES('Hello', 1, 3, datetime('now', 'localtime'));
INSERT INTO messages VALUES('Hello2', 3, 1, datetime('now', 'localtime'));
INSERT INTO messages VALUES('Whats up', 2, 3, datetime('now', 'localtime'));
INSERT INTO messages VALUES('Whats up2', 3, 2, datetime('now', 'localtime'));