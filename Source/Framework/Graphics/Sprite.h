#pragma once

#include "GraphicTypes.h"
#include "Texture.h"
#include "Color.h"
#include "../Math/Transformable.h"


namespace GameDev2D
{
    class Sprite : public Transformable
    {
    public:
        //Creates a Sprite using a reference to an already loaded Texture object, 
        //you are responsible for unloading the Texture
        Sprite(Texture* texture);

		//Creates a Sprite using a reference to an already loaded Texture object, 
		//you are responsible for unloading the Texture
		Sprite(const std::string& filename);

		//Creates a Sprite with an empty texture
		Sprite();

        //Destructor
        virtual ~Sprite();
        
        //Returns the width and height of the Sprite
        virtual float GetWidth();
        virtual float GetHeight();
        Vector2 GetSize();
        
        //Returns the Color associated with the Sprite object
        Color GetColor();

        //Sets the Color associated with the Sprite object
        virtual void SetColor(Color color);

        //Returns the Alpha associated with the Sprite object
        float GetAlpha();

        //Sets the Alpha associated with the Sprite object between 0.0-1.0
        virtual void SetAlpha(float alpha);

        //Returns the anchor point associated with the Sprite object
        Vector2 GetAnchor();

        //Sets the anchor point associated with the Sprite object
        void SetAnchor(Vector2 anchor);
        void SetAnchor(float anchorX, float anchorY);
        void SetAnchorX(float anchorX);
        void SetAnchorY(float anchorY);

        //Returns the location of the left most edge
        float GetLeftEdge();

        //Returns the location of the right most edge
        float GetRightEdge();

        //Returns the location of the top most edge
        float GetTopEdge();

        //Returns the location of the bottom most edge
        float GetBottomEdge();

        //Sets the frame of the Texture to render
        void SetFrame(Rect frame);
        
        //Returns the frame of the Texture to render
        Rect GetFrame();

        void GetUVs(Vector2 uvs[4]);
        
        //Set's the Sprite's Texture object
        void SetTexture(Texture* texture);

		//Set's the Sprite's Texture object by filename
		virtual void SetTexture(std::string filename);

        //Returns a pointer to the internal Texture
        Texture* GetTexture() const;
        
    protected:
        void OnTextureUnloaded(Texture* texture);
        void LocalTransformIsDirty() override;
        void CalculateEdges();

        //Member variables
        Texture* m_Texture;
        Rect m_Frame;
        Color m_Color;
        Vector2 m_Anchor;

        enum Drawable_Edges
        {
            LeftEdge = 0,
            RightEdge,
            TopEdge,
            BottomEdge,
            TotalEdges
        };

        float m_Edges[TotalEdges];
        bool m_EdgesCalculationDirty;
    };
}

