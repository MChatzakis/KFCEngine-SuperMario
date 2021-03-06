#include "./GridLayer.h"

//TILE COLORS HOLDER

void TileColorsHolder::Insert(Bitmap bmp, Index index) {
	if (indices.find(index) == indices.end()) {
		indices.insert(index);
		BitmapAccessPixels(bmp,
			[this](PixelMemory* mem)
		{ colors.insert(GridUtilities::GetPixel32(*mem)); }
		);
	}
}
bool TileColorsHolder::In(Color c) const
{
	return colors.find(c) != colors.end();
}

//GRID UTILITIES

	// 
 void GridUtilities::SetGridTile(GridMap* m, Dim col, Dim row, GridIndex index)
{
	(*m)[row][col] = index;
}

 GridIndex GridUtilities::GetGridTile(const GridMap* m, Dim col, Dim row)
{
	return (*m)[row][col];
}

 void GridUtilities::SetSolidGridTile(GridMap* m, Dim col, Dim row)
{
	SetGridTile(m, col, row, GRID_SOLID_TILE);
}

 void GridUtilities::SetEmptyGridTile(GridMap* m, Dim col, Dim row)
{
	SetGridTile(m, col, row, GRID_EMPTY_TILE);
}

 void GridUtilities::SetGridTileFlags(GridMap* m, Dim col, Dim row, GridIndex flags)
{
	SetGridTile(m, col, row, flags);
}

 void GridUtilities::SetGridTileTopSolidOnly(GridMap* m, Dim col, Dim row)
{
	SetGridTileFlags(m, row, col, GRID_TOP_SOLID_MASK);
}

 bool GridUtilities::CanPassGridTile(GridMap* m, Dim col, Dim row, GridIndex flags) // i.e. checks if flags set
{
	bool res = (bool)(*GetGridTileBlock(col / 4, row / 4, TILEMAP_WIDTH, **m) & flags);
	//res = GetGridTile(m, col, row) & flags;
	if (res)
		std::cout << "You go on Solid tile " << "(" << row << "," << col << ")\n";

	return !res; //now it returns 1 on empty tiles
	//return GetGridTile(m, row, col) & (flags != 0);
}

 void GridUtilities::FilterGridMotion(GridMap* m, const Rect& r, int* dx, int* dy) {
	assert(abs(*dx) <= GRID_ELEMENT_WIDTH && abs(*dy) <= GRID_ELEMENT_HEIGHT);
	// try horizontal move
	if (*dx < 0)
		FilterGridMotionLeft(m, r, dx);
	else
		if (*dx > 0)
			FilterGridMotionRight(m, r, dx);
	// try vertical move
	if (*dy < 0)
		FilterGridMotionUp(m, r, dy);
	else
		if (*dy > 0)
			FilterGridMotionDown(m, r, dy);
}

 void GridUtilities::FilterGridMotionLeft(GridMap* m, const Rect& r, int* dx) {
	auto x1 = r.x;
	auto x1_next = x1 + *dx;
	if (x1_next < 0)
		*dx = -x1; //goes full left
	else {
		auto newCol = DIV_GRID_ELEMENT_WIDTH(x1_next);
		auto currCol = DIV_GRID_ELEMENT_WIDTH(x1);
		if (newCol != currCol) {
			assert(newCol + 1 == currCol); // we really move left
			auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
			auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
			for (auto row = startRow; row <= endRow; ++row)
				if (!CanPassGridTile(m, newCol, row, GRID_RIGHT_SOLID_MASK)) {
					*dx = MUL_GRID_ELEMENT_WIDTH(currCol) - x1;
					break;
				}
		}
	}
}

 void GridUtilities::FilterGridMotionUp(GridMap* m, const Rect& r, int* dy) {
	auto y2 = r.y;
	auto y2_next = y2 + *dy;
	//auto y1_next = r.y + *dy;
	if (y2_next < 0)
		*dy = -y2; //goes full top
	else {
		auto newRow = DIV_GRID_ELEMENT_HEIGHT(y2_next);
		auto currRow = DIV_GRID_ELEMENT_HEIGHT(y2);
		if (newRow != currRow) {
			assert(newRow == currRow - 1); // we really move up
			auto startCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			auto endCol = DIV_GRID_ELEMENT_WIDTH(r.x + r.w - 1);
			for (auto col = startCol; col <= endCol; ++col)
				if (!CanPassGridTile(m, col, newRow, GRID_BOTTOM_SOLID_MASK)) {
					*dy = MUL_GRID_ELEMENT_HEIGHT(currRow) - y2;
					break;
				}
		}
	}
}

 void GridUtilities::FilterGridMotionDown(GridMap* m, const Rect& r, int* dy) {
	auto y1 = r.y + r.h - 1;
	auto y1_next = y1 + *dy;
	if (y1_next >= MAX_PIXEL_HEIGHT)
		*dy = (MAX_PIXEL_HEIGHT - 1) - y1; //goes full down
	else {
		auto newRow = DIV_GRID_ELEMENT_HEIGHT(y1_next);
		auto currRow = DIV_GRID_ELEMENT_HEIGHT(y1);
		if (newRow != currRow) {
			assert(newRow == currRow + 1); // we really move down
			auto startCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			auto endCol = DIV_GRID_ELEMENT_WIDTH(r.x + r.w - 1);
			for (auto col = startCol; col <= endCol; ++col)
				if (!CanPassGridTile(m, col, newRow, GRID_TOP_SOLID_MASK)) {
					*dy = MUL_GRID_ELEMENT_HEIGHT(newRow) - 1 - y1;
					break;
				}
		}
	}
}

 void GridUtilities::FilterGridMotionRight(GridMap* m, const Rect& r, int* dx) {
	auto x2 = r.x + r.w - 1;
	auto x2_next = x2 + *dx;
	if (x2_next >= MAX_PIXEL_WIDTH) {
		*dx = (MAX_PIXEL_WIDTH - 1) - x2; //goes full right
		std::cout << "kseperase to orio deksia\n";
	}
	else {
		auto newCol = DIV_GRID_ELEMENT_WIDTH(x2_next);
		auto currCol = DIV_GRID_ELEMENT_WIDTH(x2);
		if (newCol != currCol) {
			assert(newCol - 1 == currCol); // we really move right
			auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
			auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
			for (auto row = startRow; row <= endRow; ++row)
				if (!CanPassGridTile(m, newCol, row, GRID_LEFT_SOLID_MASK)) {
					*dx = (MUL_GRID_ELEMENT_WIDTH(newCol) - 1) - x2;
					std::cout << "synantise solid tile\n";
					break;
				}
		}
	}
}

 bool GridUtilities::IsTileIndexAssumedEmpty(Index index) {
	for (int i = 0; i < TOTAL_EMPTY_INDICES; i++) {
		if (EMPTY_INDICES[i] == index) {
			return true;
		}
	}
	return false;
}


 void GridUtilities::ComputeTileGridBlocks1(const TileMap* map, GridIndex* grid) {
	for (auto row = 0; row < TILEMAP_HEIGHT; ++row) //change
		for (auto col = 0; col < TILEMAP_WIDTH; ++col) {
			memset(grid, IsTileIndexAssumedEmpty(TileUtilities::GetTile(map, col, row)) ? GRID_EMPTY_TILE : GRID_SOLID_TILE, GRID_ELEMENTS_PER_TILE);
			//memset()
			grid += GRID_ELEMENTS_PER_TILE;
		}
}

// mexri edw evala


 bool GridUtilities::IsTileColorEmpty(Color c)
{
	return emptyTileColors.In(c);
	//return false;
} // return false to disable

//ti sto diaolo?
 void GridUtilities::ComputeTileGridBlocks2(const TileMap* map, GridIndex* grid, Bitmap tileSet, Color transColor, byte solidThreshold) {
	auto tileElem = al_create_bitmap(TILE_WIDTH, TILE_HEIGHT);
	auto gridElem = al_create_bitmap(GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT);
	for (auto row = 0; row < MAX_HEIGHT; ++row)
		for (auto col = 0; col < MAX_WIDTH; ++col) {
			auto index = TileUtilities::GetTile(map, col, row);
			TileUtilities::PutTile(tileElem, 0, 0, tileSet, index);
			if (IsTileIndexAssumedEmpty(index)) {
				emptyTileColors.Insert(tileElem, index); // assume tile colors to be empty
				memset(grid, GRID_EMPTY_TILE, GRID_ELEMENTS_PER_TILE);
				grid += GRID_ELEMENTS_PER_TILE;
			}
			else
				ComputeGridBlock(// auto increments grid as T*& 
					grid, index, tileElem, gridElem,
					tileSet, transColor, solidThreshold
				);
		}
	al_destroy_bitmap(tileElem);
	al_destroy_bitmap(gridElem);
}

//ti sto diaolo?
 void GridUtilities::ComputeGridBlock(GridIndex*& grid, Index index, Bitmap tileElem, Bitmap gridElem, Bitmap tileSet, Color transColor, byte solidThreshold) {
	for (auto i = 0; i < GRID_ELEMENTS_PER_TILE; ++i) {
		auto x = i % GRID_BLOCK_ROWS;
		auto y = i / GRID_BLOCK_ROWS;
		BitmapBlit(
			tileElem,
			{ x * GRID_ELEMENT_WIDTH, y * GRID_ELEMENT_HEIGHT, GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT },
			gridElem,
			{ 0, 0 }
		);
		//al_draw_bitmap_region((ALLEGRO_BITMAP*)tileElem, x * GRID_ELEMENT_WIDTH, y * GRID_ELEMENT_HEIGHT, GRID_ELEMENT_WIDTH, GRID_ELEMENT_HEIGHT,/*gridElem*/ 0, 0, 0);

		auto isEmpty = ComputeIsGridIndexEmpty(gridElem, transColor, solidThreshold);
		*grid++ = isEmpty ? GRID_EMPTY_TILE : GRID_SOLID_TILE;
	}
}

 Color GridUtilities::GetPixel32(PixelMemory mem) {
	RGBA c;
	ReadPixelColor32(mem, &c, &c.a);

	//return MakeColor32(c.r, c.g, c.b, c.a);
	return Make32(c.r, c.g, c.b, c.a);
}

 bool GridUtilities::ComputeIsGridIndexEmpty(Bitmap gridElement, Color transColor, byte solidThreshold) {
	auto n = 0;
	BitmapAccessPixels(
		gridElement,
		[transColor, &n](PixelMemory* mem) {
		auto c = GetPixel32(*mem);
		//std::cout << "Color inside func " << c << "\n";
		if (c != transColor && !IsTileColorEmpty(c))
			++n;
	}
	);
	return n <= solidThreshold;
}

 GridIndex* GridUtilities::GetGridTileBlock(Dim colTile, Dim rowTile, Dim tileCols, GridIndex* grid) {
	return grid + (rowTile * tileCols + colTile) * GRID_BLOCK_SIZEOF;

}

 void GridUtilities::SetGridTileBlock(Dim colTile, Dim rowTile, Dim tileCols, GridIndex* grid, GridIndex flags) {
	memset(GetGridTileBlock(colTile, rowTile, tileCols, grid), flags, GRID_BLOCK_SIZEOF);
}

// use this to render grid (toggle on / off), used only for development time testing -
// a tile grid block is consecutive GRID_BLOCK_ROWS x GRID_BLOCK_COLUMNS block of grid indices
template <typename Tfunc>  void GridUtilities::DisplayGrid(Bitmap dest, const Rect& viewWin, GridIndex* grid, Dim tileCols, const Tfunc& display_f) {
	auto startCol = DIV_TILE_WIDTH(viewWin.x);
	auto startRow = DIV_TILE_HEIGHT(viewWin.y);
	auto endCol = DIV_TILE_WIDTH(viewWin.x + viewWin.w - 1);
	auto endRow = DIV_TILE_HEIGHT(viewWin.y + viewWin.h - 1);
	for (Dim rowTile = startRow; rowTile <= endRow; ++rowTile)
		for (Dim colTile = startCol; colTile <= endCol; ++colTile) {
			auto sx = MUL_TILE_WIDTH(colTile - startCol);
			auto sy = MUL_TILE_HEIGHT(rowTile - startRow);
			auto* gridBlock = GetGridTileBlock(colTile, rowTile, tileCols, grid);
			for (auto rowElem = 0; rowElem < GRID_BLOCK_ROWS; ++rowElem)
				for (auto colElem = 0; colElem < GRID_BLOCK_COLUMNS; ++colElem)
					if (*gridBlock++ & GRID_SOLID_TILE) {
						auto x = sx + MUL_GRID_ELEMENT_WIDTH(colElem);
						auto y = sy + MUL_GRID_ELEMENT_HEIGHT(rowElem);
						auto w = GRID_ELEMENT_WIDTH;
						auto h = GRID_ELEMENT_HEIGHT;
						display_f(dest, x, y, w, h);
					}
		}
}

 void GridUtilities::DisplayGrid(Bitmap dest, const Rect& viewWin, GridIndex* grid, Dim tileCols) {
	auto startCol = DIV_TILE_WIDTH(viewWin.x);
	auto startRow = DIV_TILE_HEIGHT(viewWin.y);
	auto endCol = DIV_TILE_WIDTH(viewWin.x + viewWin.w - 1);
	auto endRow = DIV_TILE_HEIGHT(viewWin.y + viewWin.h - 1);
	for (Dim rowTile = startRow; rowTile <= endRow; ++rowTile)
		for (Dim colTile = startCol; colTile <= endCol; ++colTile) {
			auto sx = MUL_TILE_WIDTH(colTile - startCol);
			auto sy = MUL_TILE_HEIGHT(rowTile - startRow);
			auto* gridBlock = GetGridTileBlock(colTile, rowTile, tileCols, grid);
			for (auto rowElem = 0; rowElem < GRID_BLOCK_ROWS; ++rowElem)
				for (auto colElem = 0; colElem < GRID_BLOCK_COLUMNS; ++colElem) {
					//assert(gridBlock == nullptr);
					if (*gridBlock++ & GRID_SOLID_TILE) {
						auto x = sx + MUL_GRID_ELEMENT_WIDTH(colElem);
						auto y = sy + MUL_GRID_ELEMENT_HEIGHT(rowElem);
						auto w = GRID_ELEMENT_WIDTH;
						auto h = GRID_ELEMENT_HEIGHT;

						al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(0, 0, 0), 1.0);
					}
				}
		}
}

//GRID LAYER
//ok
void GridLayer::Allocate(void) {
	auto total = totalRows * totalColumns;
	grid = new GridIndex[total];
	memset(grid, GRID_EMPTY_TILE, total);
}

//ok
void  GridLayer::FilterGridMotionLeft(const Rect& r, int* dx) {
	auto x1 = r.x;
	auto x1_next = x1 + *dx;
	if (x1_next < 0)
		*dx = -x1; //goes full left
	else {
		auto newCol = DIV_GRID_ELEMENT_WIDTH(x1_next);
		auto currCol = DIV_GRID_ELEMENT_WIDTH(x1);
		if (newCol != currCol) {
			assert(newCol + 1 == currCol); // we really move left
			auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
			auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
			for (auto row = startRow; row <= endRow; ++row)
				if (!CanPassGridTile(newCol, row, GRID_RIGHT_SOLID_MASK)) {
					*dx = MUL_GRID_ELEMENT_WIDTH(currCol) - x1;
					break;
				}
		}
	}
}

//ok
void  GridLayer::FilterGridMotionUp(const Rect& r, int* dy) {
	auto y2 = r.y;
	auto y2_next = y2 + *dy;

	if (y2_next < 0)
		*dy = -y2; //goes full top
	else {
		auto newRow = DIV_GRID_ELEMENT_HEIGHT(y2_next);
		auto currRow = DIV_GRID_ELEMENT_HEIGHT(y2);
		if (newRow != currRow) {
			assert(newRow == currRow - 1); // we really move up
			auto startCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			auto endCol = DIV_GRID_ELEMENT_WIDTH(r.x + r.w - 1);
			for (auto col = startCol; col <= endCol; ++col)
				if (!CanPassGridTile(col, newRow, GRID_BOTTOM_SOLID_MASK)) {
					*dy = MUL_GRID_ELEMENT_HEIGHT(currRow) - y2;
					break;
				}
		}
	}
}

//ok
void  GridLayer::FilterGridMotionDown(const Rect& r, int* dy) {
	auto y1 = r.y + r.h - 1;
	auto y1_next = y1 + *dy;
	if (y1_next >= MAX_PIXEL_HEIGHT)
		*dy = (MAX_PIXEL_HEIGHT - 1) - y1; //goes full down
	else {
		auto newRow = DIV_GRID_ELEMENT_HEIGHT(y1_next);
		auto currRow = DIV_GRID_ELEMENT_HEIGHT(y1);
		if (newRow != currRow) {
			assert(newRow == currRow + 1); // we really move down
			auto startCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			auto endCol = DIV_GRID_ELEMENT_WIDTH(r.x + r.w - 1);
			for (auto col = startCol; col <= endCol; ++col)
				if (!CanPassGridTile(col, newRow, GRID_TOP_SOLID_MASK)) {
					*dy = MUL_GRID_ELEMENT_HEIGHT(newRow) - 1 - y1;
					break;
				}
		}
	}
}

//ok
void  GridLayer::FilterGridMotionRight(const Rect& r, int* dx) {
	auto x2 = r.x + r.w - 1;
	auto x2_next = x2 + *dx;

	if (x2_next >= MAX_PIXEL_WIDTH) {
		*dx = (MAX_PIXEL_WIDTH - 1) - x2; //goes full right
	}
	else {
		auto newCol = DIV_GRID_ELEMENT_WIDTH(x2_next);
		auto currCol = DIV_GRID_ELEMENT_WIDTH(x2);
		if (newCol != currCol) {
			assert(newCol - 1 == currCol); // we really move right
			auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
			auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
			for (auto row = startRow; row <= endRow; ++row)
				if (!CanPassGridTile(newCol, row, GRID_LEFT_SOLID_MASK)) {
					*dx = (MUL_GRID_ELEMENT_WIDTH(newCol) - 1) - x2;
					break;
				}
		}
	}
}

//ok
void  GridLayer::SetGridTile(Dim col, Dim row, GridIndex index)
{
	grid[row * totalColumns + col] = index;
}

//ok
GridIndex  GridLayer::GetGridTile(Dim col, Dim row)
{
	return grid[row * totalColumns + col ];
}

//ok
void  GridLayer::SetSolidGridTile(Dim col, Dim row)
{
	SetGridTile(col, row, GRID_SOLID_TILE);
}

//ok
void  GridLayer::SetEmptyGridTile(Dim col, Dim row)
{
	SetGridTile(col, row, GRID_EMPTY_TILE);
}

//ok
void  GridLayer::SetGridTileFlags(Dim col, Dim row, GridIndex flags)
{
	SetGridTile(col, row, flags);
}

//ok
void  GridLayer::SetGridTileTopSolidOnly(Dim col, Dim row)
{
	SetGridTileFlags(row, col, GRID_TOP_SOLID_MASK);
}

//ok
bool  GridLayer::CanPassGridTile(Dim col, Dim row, GridIndex flags) // i.e. checks if flags set
{
	//return !(GetGridTile(col, row) & flags);
	auto colTile = col / GRID_ELEMENT_WIDTH;
	auto rowTile = row / GRID_ELEMENT_HEIGHT;
	auto tileCols = this->GetTotalColumns() / GRID_ELEMENT_WIDTH;
	return !(*(GridUtilities::GetGridTileBlock(colTile, rowTile, tileCols, this->GetBuffer())) & flags);
	//if (res)
		//std::cout << "You go on Solid tile " << "(" << row << "," << col << ")\n";

	//return !res; //now it returns 1 on empty tiles
	//return GetGridTile(m, row, col) & (flags != 0);*/
}

//ok
void  GridLayer::FilterGridMotion(const Rect& r, int* dx, int* dy) {
	assert(abs(*dx) <= GRID_ELEMENT_WIDTH && abs(*dy) <= GRID_ELEMENT_HEIGHT);
	// try horizontal move
	if (*dx < 0)
		FilterGridMotionLeft(r, dx);
	else
		if (*dx > 0)
			FilterGridMotionRight(r, dx);
	// try vertical move
	if (*dy < 0)
		FilterGridMotionUp(r, dy);
	else
		if (*dy > 0)
			FilterGridMotionDown(r, dy);
}

//ok
bool  GridLayer::IsOnSolidGround(const Rect& r) { // will need later for gravity
	int dy = 1; // down 1 pixel
	FilterGridMotionDown(r, &dy);
	return dy == 0; // if true IS attached to solid ground
}

//ok
GridIndex* GridLayer::GetBuffer(void) {
	return grid;
}

//ok
bool  GridLayer::IsTileIndexAssumedEmpty(Index index) {
	for (int i = 0; i < TOTAL_EMPTY_INDICES; i++) {
		if (EMPTY_INDICES[i] == index) {
			return true;
		}
	}
	return false;
}

//ok
const GridIndex*& GridLayer::GetBuffer(void) const {
	return (const GridIndex*&)grid; //compile error, to see
}

//ok
GridLayer::GridLayer(unsigned rows, unsigned cols) {
	totalRows = rows;
	totalColumns = cols;
	Allocate();
}

Dim GridLayer::GetTotalRows() {
	return totalRows;
}

Dim GridLayer::GetTotalColumns() {
	return totalColumns;
}