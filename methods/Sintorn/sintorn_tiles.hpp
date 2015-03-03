#ifndef _SINTORN_TILES_HPP_
#define _SINTORN_TILES_HPP_

/**
 * @brief This function finds out the best distribution of size among tiles
 *
 * @param TileSize output tile sizes 
 * @param WindowSize windows size
 * @param WavefrontSize wavefront size usually 32 or 64
 *
 * @return returns number of levels
 */
unsigned ChooseTileSizes(unsigned**TileSize,unsigned*WindowSize,unsigned WavefrontSize);

#endif//_SINTORN_TILES_HPP_
