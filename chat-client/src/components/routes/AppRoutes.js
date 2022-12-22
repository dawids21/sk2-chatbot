import { Route, Routes } from "react-router-dom";
import Home from "../../pages/Home";
import Login from "../../pages/Login";
import Register from "../../pages/Register";
import ProtectedRoute from "./ProtectedRoute";

const AppRoutes = () => {
  return (
    <Routes>
      <Route
        path="/"
        element={
          <ProtectedRoute>
            <Home />
          </ProtectedRoute>
        }
      />
      <Route path="/login" element={<Login />} />
      <Route path="/register" element={<Register />} />
    </Routes>
  );
};

export default AppRoutes;
