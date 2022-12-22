import { Paper, Typography } from "@mui/material";
import { useNavigate } from "react-router-dom";

const UserList = ({ users }) => {
  const navigate = useNavigate();
  if (users.length === 0) {
    return;
  }
  return (
    <>
      {users.map((user) => (
        <Paper
          sx={{ mt: 1, p: 2, display: "flex", justifyContent: "space-between" }}
          elevation={4} key={user.id} onClick={() => navigate(`/chat/${user.id}`)}
        >
          <Typography variant="h5">
            {user.username}
          </Typography>
        </Paper>
      ))}
    </>
  );
};

export default UserList;
