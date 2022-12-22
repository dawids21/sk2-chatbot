import { Container, Paper } from "@mui/material";
import { useCallback, useContext, useEffect, useState } from "react";
import SearchBar from "../components/home/SearchBar";
import UserList from "../components/home/UserList";
import backend from "../config/backend";
import AuthContext from "../context/auth-context";
import useInput from "../hooks/use-input";
import useSnackbar from "../hooks/use-snackbar";

const Home = () => {
  const nameInput = useInput((value) => true);
  const [users, setUsers] = useState([]);
  const { token } = useContext(AuthContext);
  const alert = useSnackbar();

  const getUsers = useCallback(
    async (name) => {
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
      }
      const data = await response.json();
      setUsers(data);
    },
    [alert, token]
  );

  useEffect(() => {
    const handler = setTimeout(() => getUsers(nameInput.value), 400);
    return () => clearTimeout(handler);
  }, [nameInput.value, getUsers]);

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
            <UserList users={users} />
          </Paper>
        </Container>
      ) : null}
    </>
  );
};

export default Home;
