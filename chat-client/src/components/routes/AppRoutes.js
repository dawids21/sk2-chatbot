import { useContext } from "react";
import { Route, Routes } from "react-router-dom";
import AuthContext from "../../context/auth-context";
import Home from "../../pages/Home";
import Login from "../../pages/Login";
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
    </Routes>
  );
};

export default AppRoutes;
