import AuthContextProvider from "./context/AuthContextProvider";
import SnackbarContextProvider from "./context/SnackbarContextProvider";
import AppRoutes from "./components/routes/AppRoutes"
import AppBar from "./components/ui/appBar/AppBar";
import { ThemeProvider } from "@mui/material";
import theme from "./config/theme"

const App = () => {
  return (
    <AuthContextProvider>
      <SnackbarContextProvider>
        <ThemeProvider theme={theme}>
          <header>
            <AppBar />
          </header>
          <main>
            <AppRoutes />
          </main>
        </ThemeProvider>
      </SnackbarContextProvider>
    </AuthContextProvider>
  );
}

export default App;
