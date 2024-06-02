#if defined datawalker__included
    #endinput
#endif
#define datawalker__included

#define MAX_ITERATOR_ITEMS 10

// ќпределение структуры дл€ итератора
struct Iterator 
{
    any items[MAX_ITERATOR_ITEMS],
    int count,
    int currentPosition
}

// ћакросы дл€ работы с итератором

#define Iterator_Create() \
    { \
        .count = 0, \
        .currentPosition = 0 \
    }

#define Iterator_Add(iterator, value) \
    ( \
        (iterator).count < MAX_ITERATOR_ITEMS ? \
        (iterator).items[(iterator).count++] = (value), true : \
        false \
    )

#define Iterator_GetCurrentValue(iterator) \
    ( \
        (iterator).currentPosition >= 0 && (iterator).currentPosition < (iterator).count ? \
        (iterator).items[(iterator).currentPosition] : \
        -1 /* или другое значение, указывающее на ошибку */ \
    )

#define Iterator_MoveNext(iterator) \
    ( \
        (iterator).currentPosition < (iterator).count - 1 ? \
        ((iterator).currentPosition++, true) : \
        false \
    )

#define Iterator_RemoveCurrent(iterator) \
    ( \
        (iterator).currentPosition >= 0 && (iterator).currentPosition < (iterator).count ? \
        ( \
            for (int i = (iterator).currentPosition; i < (iterator).count - 1; i++) \
                (iterator).items[i] = (iterator).items[i + 1]; \
            (iterator).count--, true \
        ) : \
        false \
    )