import { Container, Paper } from "@mui/material";
import { useContext, useEffect, useState } from "react";
import SearchBar from "../components/home/SearchBar";
import UserList from "../components/home/UserList";
import backend from "../config/backend";
import AuthContext from "../context/auth-context";
import useInput from "../hooks/use-input";
import useSnackbar from "../hooks/use-snackbar";

const Home = () => {
  const nameInput = useInput((value) => true);
  const [users, setUsers] = useState([]);
  const [friends, setFriends] = useState([]);
  const { isLoggedIn, token, id } = useContext(AuthContext);
  const alert = useSnackbar();

  useEffect(() => {
    const getFriends = async () => {
      const response = await fetch(`${backend.url}/friends`, {
        method: "GET",
        headers: {
          Authorization: token,
        },
      });
      if (response.status !== 200) {
        alert("Something went wrong!", "error");
        return;
      }
      const data = await response.json();
      setFriends(data);
    };
    if (isLoggedIn && token !== "") {
      getFriends();
    }
  }, [alert, isLoggedIn, token]);

  useEffect(() => {
    const getUsers = async (name) => {
      if (name === "") {
        setUsers([]);
        return;
      }
      const response = await fetch(`${backend.url}/users/username`, {
        method: "POST",
        body: JSON.stringify({ username: name }),
        headers: {
          Authorization: token,
        },
      });
      if (response.status !== 200) {
        alert("Something went wrong!", "error");
        return;
      }
      const data = await response.json();
      setUsers(data.filter((user) => user.user_id !== id));
    };
    const handler = setTimeout(() => getUsers(nameInput.value), 400);
    return () => clearTimeout(handler);
  }, [nameInput.value, alert, token, id]);

  const addFriendHandler = async (event, friendId, friendUsername) => {
    event.stopPropagation();
    const response = await fetch(`${backend.url}/friends`, {
      method: "POST",
      body: JSON.stringify({ friend_id: friendId }),
      headers: {
        Authorization: token,
      },
    });
    if (response.status !== 200) {
      alert("Something went wrong!", "error");
    }
    setFriends((prevFriends) => [
      ...prevFriends,
      { user_id: friendId, username: friendUsername },
    ]);
  };

  const removeFriendHandler = async (event, friendId) => {
    event.stopPropagation();
    const response = await fetch(`${backend.url}/friends`, {
      method: "DELETE",
      body: JSON.stringify({ friend_id: friendId }),
      headers: {
        Authorization: token,
      },
    });
    if (response.status !== 200) {
      alert("Something went wrong!", "error");
    }
    setFriends((prevFriends) =>
      prevFriends.filter((friend) => friend.user_id !== friendId)
    );
  };

  return (
    <>
      <Container>
        <Paper sx={{ p: 2 }}>
          <SearchBar input={nameInput} />
        </Paper>
      </Container>
      {users.length !== 0 ? (
        <Container maxWidth="sm" sx={{ mt: 2 }}>
          <Paper sx={{ p: 2 }}>
            <UserList
              users={users}
              friends={friends}
              addFriendHandler={addFriendHandler}
              removeFriendHandler={removeFriendHandler}
            />
          </Paper>
        </Container>
      ) : null}
    </>
  );
};

export default Home;
