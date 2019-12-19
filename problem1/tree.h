#pragma once

#include <memory>

namespace bintree {
    template <typename T>
    struct TNode : std::enable_shared_from_this<TNode<T>> {
      
        using TNodePtr = std::shared_ptr<TNode<T>>;
        using TNodeConstPtr = std::shared_ptr<const TNode<T>>;

        // Указатель на предка weak_ptr,
        // чтбы избежать циклических ссылок.
        using TWeakPtr = std::weak_ptr<TNode<T>>;

        // Ради возможности передавать указатель на себя,
        // подключаем наследование от класса и расширяем поле имён
        using std::enable_shared_from_this<TNode<T>>::shared_from_this;

        bool hasLeft() const {
            return bool(left);
        }

        bool hasRight() const {
            return bool(right);
        }

        bool hasParent() const {
	  // Переписан для соответствия weak_ptr
	  return not bool(parent.expired());
        }

        T& getValue() {
            return value;
        }

        const T& getValue() const {
            return value;
        }

        TNodePtr getLeft() {
            return left;
        }

        TNodeConstPtr getLeft() const {
            return left;
        }

        TNodePtr getRight() {
            return right;
        }

        TNodeConstPtr getRight() const {
            return right;
        }

        TNodePtr getParent() {
	  // Переписан для соответствия weak_ptr
	  return parent.lock();
        }

        TNodeConstPtr getParent() const{
	  // Переписан для соответствия weak_ptr
	  return parent.lock();
        }

        static TNodePtr createLeaf(T v) {
	    // Чтобы можно было создавать объект в make_shared,
	    // make_shared должен иметь доступ к его конструктору.
	    // Исправлено на явный вызов конструктора,
	    // хотя и немного меняет семантику - make_shared
	    // старается расположить объект и управляющий блок рядом,
	    // в одном сегменте памяти.
	    // Сейчас же вызывается два раза размещение -
	    // отдельно объекта и управляющего блока.
	    TNodePtr result (new TNode(v));
            return result;
        }

        static TNodePtr fork(T v, TNodePtr left, TNodePtr right) {
	    // Переписан. Изменены параметры, и прототип использования.
	    // Параметры входа - умные указатели, а не raw_pointers
	    // Иначе возникает ошибка взаимного существования
	    // двух изолированных указателей на один объект.
	    // (Вызывает проблемы при удалении)
            TNodePtr ptr = createLeaf(v);
	    // Объект создаётся с помощью конструктора с одним параметром,
	    // затем изменяется с помощью вызова функций.
	    // Это позволяет избежать эффектов,
	    // когда новый потомок имеет предка,
	    // сохраняющего на него ссылку.
	    // Данный подход меняет изначальную логику -
	    // там ссылка сохранялась,
	    // однако не меняет семантику метода.
	    ptr->replaceLeft(left);
	    ptr->replaceRight(right);
            return ptr;
        }

        TNodePtr replaceLeft(TNodePtr l) {
	    //Поправлена особенность работы с this:
	    //ради исключения создания двух указателей на один объект,
	    //не знающих друг о друге,
	    //необходима передача shared_ptr, что обеспечивается
	    //конструкцией shared_from_this
            setParent(l, shared_from_this());
            setParent(left, nullptr);
            std::swap(l, left);
	    return l;
        }

        TNodePtr replaceRight(TNodePtr r) {
	    // --//-- replaceLeft
	    setParent(r, shared_from_this());
            setParent(right, nullptr);
            std::swap(r, right);
	    return r;
        }

        TNodePtr replaceRightWithLeaf(T v) {
            return replaceRight(createLeaf(v));
        }

        TNodePtr replaceLeftWithLeaf(T v) {
            return replaceLeft(createLeaf(v));
        }

        TNodePtr removeLeft() {
            return replaceLeft(nullptr);
        }
        TNodePtr removeRight() {
            return replaceRight(nullptr);
        }

    private:
        T value;
        TNodePtr left = nullptr;
        TNodePtr right = nullptr;
        // Указатель на родителя сделан слабым,
        // что позволяет избежать циклических ссылок.
        // Однако уже не получится присвоить ему значение nullptr.
        TWeakPtr parent = TWeakPtr();

    private:
        TNode(T v)
            : value(v)
        {}

    private:
        //Недостижимый в текущей редакции код.
        TNode(T v, TNodePtr left, TNodePtr right)
            : value(v)
            , left(left)
            , right(right)
        {}

        static void setParent(TNodePtr node, TNodePtr parent) {
	  if (node)
		node->parent = parent;
	}
    };
}
