import { Box, Button, TextField } from "@mui/material";
import Grid2 from "@mui/material/Unstable_Grid2";
import { useContext, useEffect, useState } from "react";
import { useParams } from "react-router-dom";
import ChatLog from "../components/chat/ChatLog";
import UserList from "../components/chat/UserList";
import CenterCircularProgress from "../components/ui/CenterCircularProgress";
import backend from "../config/backend";
import AuthContext from "../context/auth-context";
import useSnackbar from "../hooks/use-snackbar";

const Chat = () => {
  const { userId } = useParams();
  const [friends, setFriends] = useState([]);
  const [isLoading, setIsLoading] = useState(true);
  const { isLoggedIn, token } = useContext(AuthContext);
  const alert = useSnackbar();
  useEffect(() => {
    const getFriends = async () => {
      setIsLoading(true);
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
      setIsLoading(false);
    };
    getFriends();
  }, [alert, isLoggedIn, token]);
  return (
    <Grid2 container spacing={2} sx={{ m: 2, mb: 0 }}>
      <Grid2 xs={2}>
        {isLoading ? <CenterCircularProgress /> : <UserList users={friends} />}
      </Grid2>
      <Grid2 xs={10}>
        {userId !== undefined ? (
          <Box
            sx={{
              overflow: "hidden",
              maxHeight: "calc(100vh - 96px)",
              height: "calc(100vh - 96px)",
              display: "flex",
              flexDirection: "column",
            }}
          >
            <Box sx={{ overflowY: "auto", flexGrow: 1 }}>
              <ChatLog userId={userId} />
            </Box>
            <TextField
              margin="normal"
              id="message"
              type="text"
              fullWidth
              variant="outlined"
            />
          </Box>
        ) : null}
      </Grid2>
    </Grid2>
  );
};

export default Chat;
