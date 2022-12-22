import { TextField } from "@mui/material";

const SearchBar = ({ input }) => {
  return (
    <TextField
      margin="normal"
      id="name"
      type="text"
      fullWidth
      variant="outlined"
      value={input.value}
      onChange={input.valueChangeHandler}
      onBlur={input.inputBlurHandler}
      error={input.hasError}
    />
  );
};

export default SearchBar;
