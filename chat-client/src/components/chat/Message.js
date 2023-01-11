import { Typography } from "@mui/material";
import { forwardRef } from "react";

const Message = forwardRef(({ name, timestamp, message }, ref) => {
  return (
    <Typography variant="body1" ref={ref !== undefined ? ref : null}>
      {`${name} (${timestamp.toLocaleDateString("pl-PL", {
        year: "numeric",
        month: "numeric",
        day: "numeric",
        hour: "numeric",
        minute: "numeric",
      })}): ${message}`}
    </Typography>
  );
});

export default Message;
