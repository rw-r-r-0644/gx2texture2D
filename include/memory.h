/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#ifndef __MEMORY_H_
#define __MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <malloc.h>
#include <stdio.h>
#include <stdint.h>

static inline void *read_file_to_bytes(char *path)
{
    FILE *load_file = fopen(path, "rb");
   
    if(!load_file) return NULL;
   
    fseek(load_file, 0, SEEK_END);
    uint32_t size = ftell(load_file);
    rewind(load_file);
   
    void *data = malloc(size);
    if(!data) return NULL;
   
    fread(data, size, sizeof(uint8_t), load_file);
    fclose(load_file);
    
    return data;
}

#ifdef __cplusplus
}
#endif

#endif // __MEMORY_H_