#ifndef demo_version_h__
#define demo_version_h__

/*
 *  You can maintain this file with `mulle-project-version`
 *  version:  major, minor, patch
 */
#define DEMO_VERSION  ((0UL << 20) | (7 << 8) | 56)


static inline unsigned int   demo_get_version_major( void)
{
   return( DEMO_VERSION >> 20);
}


static inline unsigned int   demo_get_version_minor( void)
{
   return( (DEMO_VERSION >> 8) & 0xFFF);
}


static inline unsigned int   demo_get_version_patch( void)
{
   return( DEMO_VERSION & 0xFF);
}

#endif
