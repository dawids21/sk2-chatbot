import { Typography } from "@mui/material";
import React from "react";

const Logo = (props) => {
  const { sx, variant } = props;
  return (
    <Typography
      sx={{ ...sx }}
      variant={variant}
      component="h1"
    >
      Chat
    </Typography>
  );
};

export default Logo;
