import Grid2 from "@mui/material/Unstable_Grid2";
import { useContext, useEffect, useState } from "react";
import { useParams } from "react-router-dom";
import ChatLog from "../components/chat/ChatLog";
import UserList from "../components/chat/UserList";
import backend from "../config/backend";
import AuthContext from "../context/auth-context";
import useSnackbar from "../hooks/use-snackbar";

const Chat = () => {
  const { userId } = useParams();
  const [friends, setFriends] = useState([]);
  const { isLoggedIn, token } = useContext(AuthContext);
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
    getFriends();
  }, [alert, isLoggedIn, token]);
  return (
    <Grid2 container spacing={2}>
      <Grid2 xs={4}>
        <UserList users={friends} />
      </Grid2>
      <Grid2 xs={8}>
        {userId !== undefined ? <ChatLog userId={userId} /> : null}
      </Grid2>
    </Grid2>
  );
};

export default Chat;