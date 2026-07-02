// frontend/web/src/pages/Dashboard.jsx
import React from 'react';
import { Grid, Card, CardContent, Typography, Box } from '@mui/material';
import { useQuery } from 'react-query';
import axios from 'axios';

const Dashboard = () => {
  const { data, isLoading } = useQuery('dashboard-stats', async () => {
    const response = await axios.get('/api/v1/analytics/dashboard');
    return response.data;
  });

  const stats = data?.stats || { threats: 0, alerts: 0, critical: 0 };

  const statCards = [
    { title: 'Total Threats', value: stats.threats, icon: '🛡️' },
    { title: 'Active Alerts', value: stats.alerts, icon: '🔔' },
    { title: 'Critical', value: stats.critical, icon: '🚨' },
    { title: 'Online Users', value: stats.users || 0, icon: '👤' },
  ];

  return (
    <Box sx={{ p: 3 }}>
      <Typography variant="h4" sx={{ mb: 3, color: '#00d4ff' }}>
        Dashboard
      </Typography>

      <Grid container spacing={3}>
        {statCards.map((stat, index) => (
          <Grid item xs={12} sm={6} md={3} key={index}>
            <Card sx={{ background: 'rgba(26, 26, 26, 0.9)', border: '1px solid rgba(255,255,255,0.05)' }}>
              <CardContent>
                <Typography variant="h6" sx={{ color: '#888' }}>
                  {stat.icon} {stat.title}
                </Typography>
                <Typography variant="h3" sx={{ color: '#fff', mt: 1 }}>
                  {isLoading ? '...' : stat.value}
                </Typography>
              </CardContent>
            </Card>
          </Grid>
        ))}
      </Grid>
    </Box>
  );
};

export default Dashboard;
