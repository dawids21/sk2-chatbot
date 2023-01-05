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
    message_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    friend_id INTEGER,
    message TEXT,
    timestamp TEXT);

CREATE TABLE logged_in_users(
    user_id INTEGER,
    token TEXT);





INSERT INTO users(username, password) VALUES('dawids21', 'password');
INSERT INTO users(username, password) VALUES('myylek', '12345');
INSERT INTO users(username, password) VALUES('jan_kow', 'qwerty');

INSERT INTO friends VALUES(1, 2);
INSERT INTO friends VALUES(2, 1);
INSERT INTO friends VALUES(1, 3);
INSERT INTO friends VALUES(3, 1);
INSERT INTO friends VALUES(2, 3);
INSERT INTO friends VALUES(3, 2);

INSERT INTO messages(user_id, friend_id, message, timestamp) VALUES(1, 2, 'Hi', '2023-01-05T13:35:51');
INSERT INTO messages(user_id, friend_id, message, timestamp) VALUES(2, 1, 'Hi2', '2023-01-05T13:35:51');
INSERT INTO messages(user_id, friend_id, message, timestamp) VALUES(1, 3, 'Hello', '2023-01-05T13:35:51');
INSERT INTO messages(user_id, friend_id, message, timestamp) VALUES(3, 1, 'Hello2', '2023-01-05T13:35:51');
INSERT INTO messages(user_id, friend_id, message, timestamp) VALUES(2, 3, 'Whats up', '2023-01-05T13:35:51');
INSERT INTO messages(user_id, friend_id, message, timestamp) VALUES(3, 2, 'Whats up2', '2023-01-05T13:35:51');