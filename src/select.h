#ifndef SELECT_H_INCLUDED
#define SELECT_H_INCLUDED

//#include "screen.h"
#include "scene_object.h"
#include "l_button.h"
#include "bit_counter.h"
#include "l_mixerchunk.h"


//class select_screen: public screen
class select_screen: public SceneObject
{
    private:
    	bool mActive;

        LButton *gButtons;
        uint8_t button_num;

        int selection;
        int selection2;
        bool mFirstSelection;

        bit_counter mBitCounter;

        LMixerChunk mButtonMove;
        LMixerChunk mButtonDown;
        bool mDown;
        bool mButtonSpriteChange;

    public:
        select_screen(Scene* scene = nullptr);
        ~select_screen();//
        void setScene(Scene* scene);

        void setActive(bool active) {mActive = active;}

        void render();

        //set position of six button
        void setposition(int x0 ,int y0 ,int x1 ,int y1 ,int x2 ,int y2 ,int x3 ,int y3 ,
                         int x4 ,int y4 ,int x5 ,int y5 ,int x6 ,int y6 );

        bool loadFromFile(const char* path0 ,const char* path1 ,const char* path2 ,const char* path3 ,const char* path4 ,
                          const char* path5 ,const char* path6 ,const char* pathbackground ,const char* word  ,SDL_Color textColor );

        void setAlpha(uint8_t alpha);

        void change_sprite( int bit, LButtonSprite buttonstate );
        void setSpriteClips(int height, int width);

        bit_counter& getBitCounter() { return mBitCounter; }

        int getSelection(const int which) const;
        void setSelection(const int which, const int num);
        bool isFirstSelection() { mFirstSelection = !mFirstSelection; return !mFirstSelection; }

        void update();
        bool down(int which);
};

#endif // SELECT_H_INCLUDED
