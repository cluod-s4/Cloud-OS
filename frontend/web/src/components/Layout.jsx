// frontend/web/src/components/Layout.jsx
import React from 'react';
import { Outlet, useNavigate } from 'react-router-dom';
import {
  AppBar, Toolbar, Typography, IconButton, Drawer, List,
  ListItem, ListItemIcon, ListItemText, Box, Avatar, Menu, MenuItem
} from '@mui/material';
import { Dashboard, Security, Notifications, Analytics, Settings, Logout, Person } from '@mui/icons-material';
import { useAuth } from '../contexts/AuthContext';

const drawerWidth = 240;

const Layout = () => {
  const navigate = useNavigate();
  const { user, logout } = useAuth();
  const [anchorEl, setAnchorEl] = React.useState(null);

  const menuItems = [
    { text: 'Dashboard', icon: <Dashboard />, path: '/dashboard' },
    { text: 'Threats', icon: <Security />, path: '/threats' },
    { text: 'Alerts', icon: <Notifications />, path: '/alerts' },
    { text: 'Analytics', icon: <Analytics />, path: '/analytics' },
    { text: 'Settings', icon: <Settings />, path: '/settings' },
  ];

  const handleLogout = () => {
    logout();
    navigate('/login');
  };

  return (
    <Box sx={{ display: 'flex' }}>
      <Drawer variant="permanent" sx={{ width: drawerWidth, flexShrink: 0,
        '& .MuiDrawer-paper': { width: drawerWidth, background: 'rgba(10, 10, 10, 0.95)', borderRight: '1px solid rgba(255,255,255,0.05)' } }}>
        <Toolbar>
          <Typography variant="h6" sx={{ color: '#00d4ff', fontWeight: 'bold' }}>☁️ Cloud-OS</Typography>
        </Toolbar>
        <List>
          {menuItems.map((item) => (
            <ListItem button key={item.text} onClick={() => navigate(item.path)}
              sx={{ '&:hover': { background: 'rgba(0, 212, 255, 0.1)' } }}>
              <ListItemIcon sx={{ color: '#888' }}>{item.icon}</ListItemIcon>
              <ListItemText primary={item.text} sx={{ color: '#fff' }} />
            </ListItem>
          ))}
        </List>
      </Drawer>

      <Box component="main" sx={{ flexGrow: 1, bgcolor: 'background.default', minHeight: '100vh' }}>
        <AppBar position="sticky" sx={{ background: 'rgba(10, 10, 10, 0.8)', backdropFilter: 'blur(10px)',
          borderBottom: '1px solid rgba(255,255,255,0.05)' }}>
          <Toolbar>
            <Typography variant="h6" sx={{ flexGrow: 1, color: '#fff' }}>Cloud-OS Security Platform</Typography>
            <IconButton onClick={(e) => setAnchorEl(e.currentTarget)} sx={{ color: '#fff' }}>
              <Avatar sx={{ bgcolor: '#00d4ff' }}>{user?.username?.[0]?.toUpperCase() || 'U'}</Avatar>
            </IconButton>
            <Menu anchorEl={anchorEl} open={Boolean(anchorEl)} onClose={() => setAnchorEl(null)}
              sx={{ '& .MuiPaper-root': { background: 'rgba(26, 26, 26, 0.95)', border: '1px solid rgba(255,255,255,0.05)' } }}>
              <MenuItem onClick={() => setAnchorEl(null)}>
                <ListItemIcon><Person sx={{ color: '#888' }} /></ListItemIcon>
                <ListItemText primary="Profile" sx={{ color: '#fff' }} />
              </MenuItem>
              <MenuItem onClick={handleLogout}>
                <ListItemIcon><Logout sx={{ color: '#ff6b6b' }} /></ListItemIcon>
                <ListItemText primary="Logout" sx={{ color: '#ff6b6b' }} />
              </MenuItem>
            </Menu>
          </Toolbar>
        </AppBar>

        <Box sx={{ p: 3 }}>
          <Outlet />
        </Box>
      </Box>
    </Box>
  );
};

export default Layout;
