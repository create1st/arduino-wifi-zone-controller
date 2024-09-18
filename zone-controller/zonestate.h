#ifndef ZONE_STATE_H_
#define ZONE_STATE_H_

const int MAX_ZONES = 6;

struct ZoneState {
  bool enabled[MAX_ZONES];
};

#endif