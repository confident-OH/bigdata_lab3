#include "sha256.h"

#define ZDBG 0
#define SHFR(x, n) (((x) >> (n)))
#define ROTR(x, n) (((x) >> (n)) | ((x) << ((sizeof(x) << 3) - (n))))
#define ROTL(x, n) (((x) << (n)) | ((x) >> ((sizeof(x) << 3) - (n))))

#define CHX(x, y, z) (((x) &  (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) &  (y)) ^ ( (x) & (z)) ^ ((y) & (z)))

#define BSIG0(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define BSIG1(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SSIG0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
#define SSIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))

#define SHA256_BLOCK_SIZE (512/8)
#define SHA256_COVER_SIZE (SHA256_BLOCK_SIZE*2)


static uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#if ZDBG
static int zdump_hex(const uint8_t *data, int size)
{
    int i;
    int l = 32;
 
    if (data[0] == 0x21) return 0;
    
    for(i=0; i<size; i++) {
        if((i%l) == 0) {
            printf( "[%02x] ", i/l );
        }
        printf( "%02x", data[i] );
        if(((i+1)%l) == 0) {
            printf( "\n" );
        }
    }
 
    printf( "\n" );
    return 0;
}
#else
#define zdump_hex(a, b)
#endif

static int ztransform(const uint8_t *msg, uint32_t *h)
{
    uint32_t w[64];
    uint32_t a0, b1, c2, d3, e4, f5, g6, h7;
    uint32_t t1, t2;

    int i = 0;
    int j = 0;

    for (i=0; i<16; i++) {
        w[i] = msg[j]<<24 | msg[j+1]<<16 | msg[j+2]<<8 | msg[j+3];
        j += 4;
    }

    for(i=16; i<64; i++){
        w[i] = SSIG1(w[i-2])+w[i-7]+SSIG0(w[i-15])+w[i-16];
    }

    zdump_hex((uint8_t*)w, 64*4);

    a0 = h[0];
    b1 = h[1];
    c2 = h[2];
    d3 = h[3];
    e4 = h[4];
    f5 = h[5];
    g6 = h[6];
    h7 = h[7];

    for (i= 0; i<64; i++) {
        t1 = h7 + BSIG1(e4) + CHX(e4, f5, g6) + k[i] + w[i];
        t2 = BSIG0(a0) + MAJ(a0, b1, c2);

        h7 = g6;
        g6 = f5;
        f5 = e4;
        e4 = d3 + t1;
        d3 = c2;
        c2 = b1;
        b1 = a0;
        a0 = t1 + t2;
    }

    h[0] += a0;
    h[1] += b1;
    h[2] += c2;
    h[3] += d3;
    h[4] += e4;
    h[5] += f5;
    h[6] += g6;
    h[7] += h7;

    return 0;
}

int sha256(const uint8_t *src, uint32_t len, uint32_t *hash)
{
    uint8_t *tmp = (uint8_t*)src;
    uint8_t  cover_data[SHA256_COVER_SIZE];
    uint32_t cover_size = 0;

    uint32_t i = 0;
    uint32_t n = 0;
    uint32_t m = 0;
    uint32_t h[8];

    h[0] = 0x6a09e667;
    h[1] = 0xbb67ae85;
    h[2] = 0x3c6ef372;
    h[3] = 0xa54ff53a;
    h[4] = 0x510e527f;
    h[5] = 0x9b05688c;
    h[6] = 0x1f83d9ab;
    h[7] = 0x5be0cd19;

    memset(cover_data, 0x00, sizeof(uint8_t)*SHA256_COVER_SIZE);

    n = len / SHA256_BLOCK_SIZE;
    m = len % SHA256_BLOCK_SIZE;

    if (m < 56 ) {
        cover_size = SHA256_BLOCK_SIZE;
    }else {
        cover_size = SHA256_BLOCK_SIZE*2;
    }

    if (m != 0) {
        memcpy(cover_data, tmp + (n * SHA256_BLOCK_SIZE), m);
    }
    cover_data[m] = 0x80;
    cover_data[cover_size-4]  = ((len*8)&0xff000000) >> 24;
    cover_data[cover_size-3]  = ((len*8)&0x00ff0000) >> 16;
    cover_data[cover_size-2]  = ((len*8)&0x0000ff00) >> 8;
    cover_data[cover_size-1]  = ((len*8)&0x000000ff);

    zdump_hex(tmp, len-m);
    zdump_hex(cover_data, cover_size);

    for (i=0; i<n; i++) {
        ztransform(tmp, h);
        tmp += SHA256_BLOCK_SIZE;
    }

    tmp = cover_data;
    n = cover_size / SHA256_BLOCK_SIZE;
    for (i=0; i<n; i++) {
        ztransform(tmp, h);
        tmp += SHA256_BLOCK_SIZE;
    }

    if (NULL != hash) {
        memcpy(hash, h, sizeof(uint32_t)*8);
    }
    return 0;
}