// frontend/web/src/services/auth.js
import api from './api';

export const authService = {
  async login(username, password) {
    const response = await api.post('/api/v1/auth/login', { username, password });
    return response.data;
  },

  async register(userData) {
    const response = await api.post('/api/v1/auth/register', userData);
    return response.data;
  },

  async getCurrentUser() {
    const response = await api.get('/api/v1/users/me');
    return response.data;
  },

  logout() {
    localStorage.removeItem('token');
    delete api.defaults.headers.common['Authorization'];
  }
};
