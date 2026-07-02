// frontend/web/src/pages/Threats.jsx
import React, { useState } from 'react';
import {
  Box, Typography, Table, TableBody, TableCell, TableContainer,
  TableHead, TableRow, Paper, Chip, IconButton, TextField, MenuItem
} from '@mui/material';
import { Visibility, CheckCircle } from '@mui/icons-material';
import { useQuery } from 'react-query';
import axios from 'axios';
import { formatDistanceToNow } from 'date-fns';

const Threats = () => {
  const [filter, setFilter] = useState({ severity: '', status: '' });

  const { data: threats, isLoading } = useQuery(['threats', filter], async () => {
    const params = new URLSearchParams();
    if (filter.severity) params.append('severity', filter.severity);
    if (filter.status) params.append('status', filter.status);
    const response = await axios.get(`/api/v1/threats?${params.toString()}`);
    return response.data;
  });

  const getSeverityColor = (severity) => {
    switch (severity) {
      case 'CRITICAL': return 'error';
      case 'HIGH': return 'warning';
      case 'MEDIUM': return 'info';
      case 'LOW': return 'success';
      default: return 'default';
    }
  };

  return (
    <Box sx={{ p: 3 }}>
      <Typography variant="h4" sx={{ mb: 3, color: '#00d4ff' }}>
        Threats
      </Typography>

      <Box sx={{ display: 'flex', gap: 2, mb: 3 }}>
        <TextField select size="small" label="Severity" value={filter.severity}
          onChange={(e) => setFilter({ ...filter, severity: e.target.value })} sx={{ minWidth: 120 }}>
          <MenuItem value="">All</MenuItem>
          <MenuItem value="CRITICAL">Critical</MenuItem>
          <MenuItem value="HIGH">High</MenuItem>
          <MenuItem value="MEDIUM">Medium</MenuItem>
          <MenuItem value="LOW">Low</MenuItem>
        </TextField>
        <TextField select size="small" label="Status" value={filter.status}
          onChange={(e) => setFilter({ ...filter, status: e.target.value })} sx={{ minWidth: 120 }}>
          <MenuItem value="">All</MenuItem>
          <MenuItem value="ACTIVE">Active</MenuItem>
          <MenuItem value="RESOLVED">Resolved</MenuItem>
        </TextField>
      </Box>

      <TableContainer component={Paper} sx={{ background: 'rgba(26, 26, 26, 0.9)' }}>
        <Table>
          <TableHead>
            <TableRow>
              <TableCell sx={{ color: '#888' }}>ID</TableCell>
              <TableCell sx={{ color: '#888' }}>Type</TableCell>
              <TableCell sx={{ color: '#888' }}>Severity</TableCell>
              <TableCell sx={{ color: '#888' }}>Status</TableCell>
              <TableCell sx={{ color: '#888' }}>Detected</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {threats?.map((threat) => (
              <TableRow key={threat.id}>
                <TableCell sx={{ color: '#fff' }}>#{threat.id}</TableCell>
                <TableCell sx={{ color: '#fff' }}>{threat.threat_type}</TableCell>
                <TableCell>
                  <Chip label={threat.severity} color={getSeverityColor(threat.severity)} size="small" />
                </TableCell>
                <TableCell>
                  <Chip label={threat.status} size="small" />
                </TableCell>
                <TableCell sx={{ color: '#fff' }}>
                  {formatDistanceToNow(new Date(threat.detected_at), { addSuffix: true })}
                </TableCell>
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </TableContainer>
    </Box>
  );
};

export default Threats;
