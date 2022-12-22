import { CircularProgress, Grid } from "@mui/material";

const CenterCircularProgress = (props) => {
  const { sx } = props;
  return (
    <Grid
      container
      spacing={0}
      direction="column"
      alignItems="center"
      justifyContent="center"
    >

      <Grid>
        <CircularProgress
          sx={{
            ...sx,
          }}
        />
      </Grid>

    </Grid>
  );
};

export default CenterCircularProgress;
