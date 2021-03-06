#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOM_SIZE 1024
enum TileType {
  FLAT,
  WALL,
  SLOPE_NORTH,
  SLOPE_EAST,
  SLOPE_SOUTH,
  SLOPE_WEST,
  SLOPE_NORTH_EAST,
  SLOPE_SOUTH_EAST,
  SLOPE_SOUTH_WEST,
  SLOPE_NORTH_WEST,
  TILE_TYPE_LEN
};

typedef struct {
  float x;
  float y;
} Point;

typedef struct {
  int id;
  char c;
  enum TileType tile_type;
} Tile;

typedef struct {
  int id;
  int width;
  int height;
  Tile *tiles;
} RoomLayer;

typedef struct {
  int id;
  int layer_count;
  RoomLayer *layers;
} Room;

char lookup[TILE_TYPE_LEN] = {'.', '#', '^', '^', '^', '^'};

Point cartesian_to_isometric(Point cart) {
  Point iso;
  iso.x = cart.x - cart.y;
  iso.y = (cart.x + cart.y) / 2;
  return iso;
}

Point isometroc_to_cartesian(Point iso) {
  Point cart;
  cart.x = (2 * iso.y + iso.x) / 2;
  cart.y = (2 * iso.y - iso.x) / 2;
  return cart;
}

bool valid_room_char(char c) { return c == '.' || c == '#' || c == '^'; }
void free_room(Room room) {
  int i;
  for (i = 0; i < room.layer_count; i++) {
    RoomLayer layer = room.layers[i];
    free(layer.tiles);
  }
  free(room.layers);
}

Room create_room(char *room_str, size_t room_str_size) {
  int layer_count = 0;
  int layer_width = 0;
  int layer_height = 0;
  bool layer_width_gathered = false;
  bool layer_height_gathered = false;
  int i;
  for (i = 0; i < room_str_size; i++) {
    char current_char = room_str[i];
    char next_char = room_str[i + 1];
    if (current_char == '-' && next_char == '\n') {
      layer_count++;
    }
    if (current_char == '\n' && layer_width > 0) {
      layer_width_gathered = true;
      if (!layer_height_gathered &&
          (valid_room_char(next_char) || next_char == EOF)) {
        layer_height++;
      } else if (!layer_height_gathered &&
                 (layer_height > 0 && next_char == '-')) {
        layer_height++;
        layer_height_gathered = true;
      }
    }
    if (valid_room_char(current_char) && !layer_width_gathered) {
      layer_width++;
    }
  }

  Room room;
  room.id = 421;
  room.layer_count = layer_count;
  room.layers = (RoomLayer *)calloc(layer_count, sizeof(RoomLayer));
  int cc = 0;
  for (i = 0; i <= layer_count; i++) {
    RoomLayer layer;
    layer.id = i;
    layer.width = layer_width;
    layer.height = layer_height;
    layer.tiles = (Tile *)calloc(layer_width * layer_height, sizeof(Tile));
    int tiles_placed = 0;
    while (tiles_placed < layer_width * layer_height) {
      char current_char = room_str[cc];
      if (current_char == '\n' || current_char == '-') {
        cc++;
        continue;
      }
      Tile tile;
      switch (current_char) {
      case '.':
        tile.tile_type = FLAT;
        break;
      case '#':
        tile.tile_type = WALL;
        break;
      case '^':
        tile.tile_type = SLOPE_NORTH;
        break;
      default:
        tile.tile_type = SLOPE_NORTH;
        break;
      }
      tile.id = cc + tiles_placed;
      tile.c = current_char;
      layer.tiles[tiles_placed] = tile;
      tiles_placed++;
      cc++;
    }
    room.layers[i] = layer;
  }

  return room;
}

void print_room(Room room) {
  int i;
  for (i = 0; i < room.layer_count; i++) {
    RoomLayer layer = room.layers[i];
    int x, y;
    for (y = 0; y < layer.height; y++) {
      for (x = 0; x < layer.width; x++) {
        printf("%c", layer.tiles[y * layer.width + x].c);
      }
      printf("\n");
    }
    printf("\n");
  }
}

void read_room_file(char *file_name) {
  FILE *file = NULL;
  file = fopen(file_name, "r");
  if (file) {
    char buff[MAX_ROOM_SIZE];
    size_t file_size = fread(buff, sizeof(char), MAX_ROOM_SIZE, file);
    Room room = create_room(buff, file_size);
    print_room(room);
    free_room(room);
  }
  fclose(file);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Please provide a level.\n");
    return 1;
  }
  int i;
  for (i = 1; i < argc; i++) {
    printf("%s\n", argv[i]);
    char *file_name = argv[i];
    read_room_file(file_name);
  }
  return 0;
}
