import { Button, Paper, Typography } from "@mui/material";

const UserList = ({ users }) => {
  if (users.length === 0) {
    return;
  }
  return (
    <>
      {users.map((user) => (
        <Paper
          sx={{ p: 2, display: "flex", justifyContent: "space-between" }}
          elevation={4}
        >
          <Typography variant="h5" key={user.id}>
            {user.username}
          </Typography>
          <Button variant="outlined">Add</Button>
          {/* <Button variant="outlined" disabled>
            Added
          </Button> */}
        </Paper>
      ))}
    </>
  );
};

export default UserList;
