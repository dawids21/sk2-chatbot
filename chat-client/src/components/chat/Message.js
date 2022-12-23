import { Typography } from "@mui/material";

const Message = ({ name, timestamp, message }) => {
  return (
    <Typography variant="body1">
      {`${name} (${timestamp.toLocaleDateString("pl-PL", {
        year: "numeric",
        month: "numeric",
        day: "numeric",
        hour: "numeric",
        minute: "numeric",
      })}): ${message}`}
    </Typography>
  );
}

export default Message;