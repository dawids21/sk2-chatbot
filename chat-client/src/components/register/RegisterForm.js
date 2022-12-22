import { Button, Link, TextField, Typography } from "@mui/material";
import { useNavigate } from "react-router-dom";
import backend from "../../config/backend";
import useInput from "../../hooks/use-input";
import useSnackbar from "../../hooks/use-snackbar";

const RegisterForm = () => {
  const usernameInput = useInput((value) => value !== "");
  const passwordInput = useInput((value) => value !== "");
  const repeatPasswordInput = useInput(
    (value) => value !== "" && value === passwordInput.value
  );
  const alert = useSnackbar();
  const navigate = useNavigate();

  const resetInputs = () => {
    usernameInput.reset();
    passwordInput.reset();
    repeatPasswordInput.reset();
  };
  const isInputValid =
    usernameInput.isValid &&
    passwordInput.isValid &&
    repeatPasswordInput.isValid;
  const handleSubmit = async (event) => {
    event.preventDefault();
    if (!isInputValid) {
      return;
    }
    const response = await fetch(`${backend.url}/register`, {
      method: "POST",
      body: JSON.stringify({
        username: usernameInput.value,
        password: passwordInput.value,
      }),
    });
    if (response.status !== 200) {
      alert("Could not register. Try again", "error");
    }
    alert("You've registered successfully", "success");
    resetInputs();
    navigate("/login");
  };

  return (
    <form onSubmit={handleSubmit}>
      <TextField
        margin="normal"
        id="username"
        label="Username"
        type="text"
        fullWidth
        variant="outlined"
        value={usernameInput.value}
        onChange={usernameInput.valueChangeHandler}
        onBlur={usernameInput.inputBlurHandler}
        error={usernameInput.hasError}
      />
      <TextField
        margin="normal"
        id="password"
        label="Password"
        type="password"
        fullWidth
        variant="outlined"
        value={passwordInput.value}
        onChange={passwordInput.valueChangeHandler}
        onBlur={passwordInput.inputBlurHandler}
        error={passwordInput.hasError}
      />
      <TextField
        margin="normal"
        id="repeatPassword"
        label="Repeat password"
        type="password"
        fullWidth
        variant="outlined"
        value={repeatPasswordInput.value}
        onChange={repeatPasswordInput.valueChangeHandler}
        onBlur={repeatPasswordInput.inputBlurHandler}
        error={repeatPasswordInput.hasError}
      />

      <Typography variant="body2">
        Already have an account? <Link href="/login">Login now.</Link>
      </Typography>
      <Button type="submit" variant="contained" fullWidth sx={{ mt: 1 }}>
        Register
      </Button>
    </form>
  );
};

export default RegisterForm;
