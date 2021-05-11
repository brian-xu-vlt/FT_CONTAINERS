#ifndef MAP_HPP
# define MAP_HPP

# include "map_pair.hpp"
# include "map_iterator.hpp"

# include <iostream>
# include <memory>
# include <cstddef>
# include <string>
# include <iterator>
# include <limits>
# include <algorithm>
# include <stdexcept>

#ifndef DEBUG_MODE
# define DEBUG_MODE 0
#endif



namespace ft	{

/*############################################################################*/
/*############################################################################*/
/*###########################                             ####################*/
/*###########################                             ####################*/
/*###########################           MAP CLASS         ####################*/
/*###########################                             ####################*/
/*###########################                             ####################*/
/*############################################################################*/
/*############################################################################*/


	template< 	class Key,
				class T,
				class Compare = std::less<Key>,
				class Allocator = std::allocator< ft::pair<const Key, T> > >
	class map {


/******************************************************************************.
.******************************************************************************.
.*********** MEMBER TYPES            ******************************************.
.******************************************************************************.
.******************************************************************************/


		private:
			struct	map_node;
		public:
			typedef Key										key_type;
			typedef T										mapped_type;
			typedef ft::pair<const Key, T>					value_type;
			typedef	Compare									key_compare;

			typedef size_t									size_type;
			typedef std::ptrdiff_t							difference_type;

			typedef Allocator								allocator_type;
			typedef typename Allocator::reference			reference;
			typedef typename Allocator::const_reference		const_reference;
			typedef typename Allocator::pointer				pointer;
			typedef typename Allocator::const_pointer		const_pointer;

			// typedef mapIterator<ft::pair<const Key, T> >	iterator;
			typedef mapIterator<Key, T, Compare, map_node>	iterator;
			typedef const mapIterator<Key, T, Compare, map_node>	const_iterator;
			// typedef std::reverse_iterator<iterator> 		reverse_iterator;
			// typedef const reverse_iterator				const_reverse_iterator;

		private:
			typedef typename Allocator::template rebind<map_node>::other	_node_allocator_type;


/******************************************************************************.
.******************************************************************************.
.*********** NESTED CLASSES          ******************************************.
.******************************************************************************.
.******************************************************************************/


			class value_compare {

				public:
					typedef	bool		result_type;
					typedef	value_type	first_argument_type;
					typedef	value_type	second_argument_type;

					bool
					operator()( const value_type& lhs, const value_type& rhs ) const {
						return (_comp(lhs.first, rhs.first));
					}

				protected:
					value_compare( Compare c ) : _comp(c) {}

					Compare				_comp;
			};

		private:
			struct map_node	{
				map_node*		left;
				map_node*		parent;
				map_node*		right;
				value_type*		item;
			};


/******************************************************************************.
.******************************************************************************.
.*********** CONSTRUCTORS DESTRUCTOR           ********************************.
.******************************************************************************.
.******************************************************************************/


		public:
			/**
			 * @brief Default Constructor
			*/
			explicit
			map( const Compare& _comp = key_compare(),
			const allocator_type & userAlloc = allocator_type() )	: 	_head(NULL),
																		_size(0),
																		_allocNode(userAlloc),
																		_allocPair(userAlloc),
																		_comp(_comp)				{

				if (DEBUG_MODE >= 2) std::cout << "CONSTRUCTOR --> DEFAULT explicit " << __func__ << std::endl;



				// debugPrintTree(_head);
			}


			/**
			 * @brief Range Constructor
			*/
			// template <class InputIterator>
			// map (InputIterator first, InputIterator last,
			// 	const key_compare& _comp = key_compare(),
			// 	const allocator_type& userAlloc = allocator_type() )
			// 	 : btree(_comp, userAlloc)	{

			// 	if (DEBUG_MODE >= 1) std::cout << "CONSTRUCTOR --> range pre dispatcher ! " << __func__ << std::endl;

			// 	iterator cursor = first;
			// 	while (cursor != last)	{
			// 		cursor++;
			// 	}
			// 	// typename std::__is_integer<InputIterator>::__type	integer;
			// 	// map_constructor_dispatch(first, last, userAlloc, integer);
			// }

			// /**
			//  * @brief Copy Constructor
			// */
			// explicit map( map const & src ) : btree(src._size()) {

			// 	if (DEBUG_MODE >= 1) std::cout << "CONSTRUCTOR --> copy " << __func__ << std::endl;

			// 	fillmap(src.begin(), src.end());
			// }


			~map( void )	{

				if (DEBUG_MODE >= 1) std::cout << "DESTRUCTOR --> " << __func__ << std::endl;
				// if (_size() > 0)
				if (_size > 0)
					clearObject();
			}

/******************************************************************************.
.******************************************************************************.
.*********** PRIVATE VARIABLES  ***********************************************.
.******************************************************************************.
.******************************************************************************/

			private:
				map_node*				_head;
				size_t					_size;
				_node_allocator_type 	_allocNode;
				allocator_type 			_allocPair;
				Compare	const			_comp;

/******************************************************************************.
.******************************************************************************.
.*********** 🚧  DEBUG 🚧             ******************************************.
.******************************************************************************.
.******************************************************************************/

			// private:
			public:
				void
				debugPrintTree()	{

					if (DEBUG_MODE < 2)
						return ;
					std::cout << "***********************************" << std::endl;

					std::cout << __func__ << "_head is pointing to:  " << _head << std::endl;
					std::cout << __func__ << "Printing tree of size  " << _size << std::endl;

					iterator it = begin();
					iterator ite = end();

					for(it; it != ite; it++)
						debugPrintNode(it._ptr);
					debugPrintNode(it._ptr);
					std::cout << "***********************************" << std::endl;
					// (void)node;
					btree_apply_node_infix(_head, debugPrintNode);
					std::cout << "***********************************" << std::endl;
				}

				static void
				debugPrintNode( map_node* node )	{

					if (DEBUG_MODE < 2)
						return ;
					if (node != NULL)	{
						std::cout << std::endl;
						std::cout << __func__ << std::endl;
						std::cout << __func__ << ": node   " << node << std::endl;
						std::cout << __func__ << ":---- KEY    " << node->item->first << std::endl;
						std::cout << __func__ << ":---- ITEM   " << node->item->second << std::endl;
						std::cout << __func__ << ": parent " << node->parent;
						if (node->parent != NULL)
							std::cout << "(" << node->parent->item->first << ";" << node->parent->item->second << ")" << std::endl;
						else
							std::cout << "--> THIS NODE IS _HEAD" << std::endl;

						std::cout << __func__ << ": left   " << node->left;
						if (node->left != NULL)
							std::cout << "(" << node->left->item->first << ";" << node->left->item->second << ")";
						std::cout << std::endl;
						std::cout << __func__ << ": right  " << node->right;
						if (node->right != NULL)
							std::cout << "(" << node->right->item->first << ";" << node->right->item->second << ")";
						std::cout << std::endl;
						std::cout << std::endl;
					}
					else
						std::cout << __func__ << "Called with null" << std::endl;

				}

/******************************************************************************.
.******************************************************************************.
.*********** PUBLIC MEMBER FUNCTIONS ******************************************.
.******************************************************************************.
.******************************************************************************/

		public:
// 			size_type
// 			max_size( void ) const	{ return this->_alloc.max_size();  }

// 			bool
// 			empty( void ) const		{ return (_size() == 0); }

// 			size_type
// 			_size( void ) const 		{ return (this->tail - this->_head); }

			iterator
			begin( void ) 			{ return (getFarLeft(_head)); }

// 			const_iterator
// 			begin( void ) const		{ return (this->_head); }

			iterator
			end( void ) 	 		{ return (getFarRight(_head)); }

// 			const_iterator
// 			end( void ) const 		{ return (this->tail); }

// 			reverse_iterator
// 			rbegin( void ) 			{ return reverse_iterator(end()); }

// 			const_reverse_iterator
// 			rbegin( void ) const	{ return reverse_iterator(end()); }

// 			reverse_iterator
// 			rend( void ) 	 		{ return reverse_iterator(begin()); }

// 			const_reverse_iterator
// 			rend( void ) const 		{ return reverse_iterator(begin()); }

// 			reference
// 			front( void ) 			{ return (*(this->_head)); }

// 			const_reference
// 			front( void ) const		{ return (*(this->_head)); }

// 			reference
// 			back( void ) 	 		{ return (*(this->tail - 1)); }

// 			const_reference
// 			back( void ) const 		{ return (*(this->tail - 1)); }

// 			size_type
// 			capacity( void ) const	{ return (this->tailStorage - this->_head); }

// 			void
// 			pop_back( void )		{ if (_size() > 0) erase(--end()); }

// 			void
// 			push_back (mapped_type const & val)	{ insert(end(), val); }

// 			void
// 			clear( void )			{ erase(begin(), end()); }

			/**
			 * @brief insert single element
			*/
			ft::pair<iterator, bool>
			insert(const value_type& val)	{

				return(btree_insert_data(NULL, &_head, val));
			}

			/**
			 * @brief insert single element with hint to determine a closer
			 * insertion point
			*/
			iterator
			insert (iterator position, const value_type& val)	{

				if(position == NULL)
					return (NULL);		// what to return ?

				ft::pair<iterator, bool> ret;
				if (position._ptr->parent != NULL
					&& (_comp(position._ptr->parent->item->first, val.first) == true
					|| _comp(val.first, position._ptr->parent->item->first) == true))
						ret = btree_insert_data(NULL, &_head, val);
				else
					ret = btree_insert_data(position._ptr, &position._ptr, val);
				return (ret.first);
			}

				/**
				 * @brief Range insert from first to last element
				*/

				// to be improved with rebalanced nodes
			template <class InputIterator>
 			void
			insert (InputIterator first, InputIterator last)	{

				iterator	lastInsert = insert(*first).first;
				first++;
				for (first; first != last; first++)
					lastInsert = insert(lastInsert, *first);
			}

// 			reference
// 			at (size_type n)	{
// 				checkRange(n);
// 				return ((*this)[n]);
// 			}

// 			const_reference
// 			at (size_type n) const	{
// 				checkRange(n);
// 				return ((*this)[n]);
// 			}

// 			/**
// 			 * @brief insert range of elements
// 			 * check here if arguments of type InputIterator are integer
// 			 * or iterators and dispatch to the right overload.
// 			 * Cf. /usr/include/c++/7/bits/cpp_type_traits.h line 136
// 			 * for implementation. Basicly __is_integer is a template function
// 			 * which, by default containts __false_type as __type, unless
// 			 * it is one of the integer types so it contains __true_type
// 			 * Here we create integer wich will be of type __true_type
// 			 * or __false_type, and by calling insert_dispatch with it, the right
// 			 * overload is called.
// 			*/
// 			template <class InputIterator>
// 			void
// 			insert (iterator position, InputIterator first,
// 			InputIterator last)	{

// 				typename std::__is_integer<InputIterator>::__type	integer;
// 				insert_dispatch(position, first, last, integer);
// 			}

// 			iterator
// 			erase (iterator position)	{

// 				destroyObjects(position._ptr, 1);
// 				memMoveLeft(position + 1, end(), 1);
// 				this->tail -= 1;
// 				return position;
// 			}

// 			iterator
// 			erase (iterator first, iterator last)	{

// 				size_type			offset = first - begin();
// 				difference_type		len = last - first;
// 				destroyObjects(first._ptr, len);
// 				memMoveLeft(last, end(), len);
// 				this->tail -= len;
// 				return begin() + offset;
// 			}

// 			void
// 			resize (size_type n, mapped_type val = mapped_type())	{

// 				if (n < _size())	{
// 					erase(end() - (_size() - n), end());
// 				}
// 				else if (n > _size())	{
// 					insert(end(), n - _size(), val);
// 				}
// 			}

// 			void
// 			swap (map& src)	{

// 				pointer 	headTmp = src._head;
// 				pointer 	tailTmp = src.tail;
// 				pointer		tailStorageTmp = src.tailStorage;

// 				src._head = this->_head;
// 				src.tail = this->tail;
// 				src.tailStorage = this->tailStorage;

// 				this->_head = headTmp;
// 				this->tail = tailTmp;
// 				this->tailStorage = tailStorageTmp;
// 			}

// 			/**
// 			 * @brief Here we use the same technic as for insert : the type
// 			 *  integer is  used to call the right overload.
// 			*/
// 			template <class InputIterator>
// 			void assign (InputIterator first, InputIterator last)	{

// 				typename std::__is_integer<InputIterator>::__type	integer;
// 				assign_dispatch(first, last, integer);
// 			}

// 			void assign(size_type n, const mapped_type& val)	{

// 				clear();
// 				insert(begin(), n, val);
// 			}

// 			/**
// 			 * @brief Reserve: Requests that the map capacity be at
// 			 * least enough to contain n elements.
// 			*/
// 			void
// 			reserve (size_type n) {

// 				if (n > max_size())	{
// 					throw std::length_error("ft::map::reserve called with n > max_size");
// 				}
// 				else	{
// 					doReserve(n);
// 				}
// 			}

// 			map&
// 			operator= (const map& x)	{

// 				if (*this != x)
// 					assign(x.begin(), x.end());
// 				return *this;
// 			}

			// T&
			// operator[]( const Key& key )	{

			// 	pointer	cursor = btree::locateKeyPosition(key);
			// 	if (cursor == NULL)
			// 		std::cout <<  "NOT FOUND" << std::endl;
			// 	else
			// 		std::cout << "FOUND: " << cursor->getPair().first << " - " << cursor->getPair().second << std::endl;
			// 	return (cursor->getPair().second);
			// }

			// const_reference
			// operator[] (size_type n) const	{
			// 	return (*(this->_head + n));
			// }

// /******************************************************************************.
// .******************************************************************************.
// .*********** PRIVATE MEMBER FUNCTIONS AND HELPERS  ****************************.
// .******************************************************************************.
// .******************************************************************************/

 		private:

			map_node*
			btree_create_node(map_node* parent, key_type k, mapped_type val)	{

				map_node*	newNode = _allocNode.allocate(1);
				_allocNode.construct(newNode, map_node());
				newNode->item = _allocPair.allocate(1);
				_allocPair.construct(newNode->item, value_type(k, val));
				newNode->parent = parent;
				return (newNode);
			}

				/**
				 * @brief Actual function inserting new data in the tree.
				 * @param parent shall be NULL to insert anywhere from the head.
				 * @param root starting point in the tree to look for a suitable potision.
				 * @param pairSrc pair to be inserted.
				*/

			ft::pair<iterator, bool>
			btree_insert_data(map_node* parent, map_node **root, value_type pairSrc)	{

				if (*root != NULL)	{
					map_node* tree = *root;
					if (_comp(pairSrc.first, tree->item->first) == true)
						return (btree_insert_data(tree, &tree->left, pairSrc));
					else if (pairSrc.first != tree->item->first)
						return (btree_insert_data(tree, &tree->right, pairSrc));
					else
						return (ft::pair<iterator, bool>(iterator(*root), false));
				}
				else	{
					*root = btree_create_node(parent, pairSrc.first, pairSrc.second);
					incSize();
					return (ft::pair<iterator, bool>(iterator(*root), true));
				}
			}

		void
			btree_apply_item_prefix(map_node *root, void (*applyf)(value_type*))	{

				if (root == NULL)
					return;
				applyf(root->item);
				btree_apply_item_prefix(root->left, applyf);
				btree_apply_item_prefix(root->right, applyf);
			}


			void
			btree_apply_item_infix(map_node *root, void (*applyf)(value_type*))	{

				if (root == NULL)
					return;
				btree_apply_item_prefix(root->left, applyf);
				applyf(root->item);
				btree_apply_item_prefix(root->right, applyf);
			}

			void
			btree_apply_item_suffix(map_node *root, void (*applyf)(value_type*))	{

				if (root == NULL)
					return;
				btree_apply_item_prefix(root->left, applyf);
				btree_apply_item_prefix(root->right, applyf);
				applyf(root->item);
			}

			void
			btree_apply_node_prefix(map_node *root, void (*applyf)(map_node *))	{

				if (root == NULL)
					return;
				applyf(root);
				btree_apply_node_prefix(root->left, applyf);
				btree_apply_node_prefix(root->right, applyf);
			}

			void
			btree_apply_node_infix(map_node *root, void (*applyf)(map_node *))	{

				if (root == NULL)
					return;
				btree_apply_node_prefix(root->left, applyf);
				applyf(root);
				btree_apply_node_prefix(root->right, applyf);
			}

			void
			btree_apply_node_suffix(map_node *root, void (*applyf)(map_node *))	{

				if (root == NULL)
					return;
				btree_apply_node_prefix(root->left, applyf);
				btree_apply_node_prefix(root->right, applyf);
				applyf(root);
			}


			value_type*
			btree_search_pair(map_node *root, value_type* targetPair)	{

				value_type*		foundTarget = NULL;
				if (root != NULL && targetPair != NULL)	{
					foundTarget = btree_search_pair(root->left. targetPair);
					if (_comp(root->item.first, targetPair->first) == 0)
						return (root->item);
					foundTarget = btree_search_pair(root->left. targetPair);
				}
				return (foundTarget);
			}

			// size_t
			// tree_nodes_count(map_node *root)	{

			// }
			// void
			// btree_free_suffix(map_node *root)	{

			// }
			// size_t
			// tree_level_count(map_node *root)	{

			// }
			// void
			// btree_apply_by_level(map_node *root,
// 			void (*applyf)(void *item, int current_level, int is_first_elem))	{

// }


			map_node*
			getFarLeft( map_node* cursor )	{

				while (cursor != NULL && cursor->left != NULL)
					cursor = cursor->left;
				return (cursor);
			}

			map_node*
			getFarRight( map_node* cursor )	{

				while (cursor != NULL && cursor->right != NULL)
					cursor = cursor->right;
				return (cursor);
			}


			// bool
			// isRightEligible(pair_type& existingPair, pair_type& newPair)	{
			// 	return (comp(existingPair.first, newPair.first));
			// }

			// bool
			// isRightEligible(Key& existingKey, Key& newKey)	{
			// 	return (comp(existingKey, newKey));
			// }

			// bool
			// isEqualKey(Key& existingKey, Key& newKey)	{
			// 	return (comp(existingKey, newKey) == false
			// 	&& comp(newKey, existingKey) == false);
			// }


			size_t
			incSize( size_t inc = 1 ) { _size += inc; return(_size + inc); }

			size_t
			decSize( size_t inc = 1 ) { _size += inc; return(_size - inc); }







// 			template <class InputIterator>
// 			void
// 			assign_dispatch (InputIterator first, InputIterator last, std::__false_type)	{

// 				clear();
// 				insert(begin(), first, last);
// 			}

// 			template<typename integer>
// 			void
// 			assign_dispatch (integer n, integer val, std::__true_type)	{

// 				assign(static_cast<size_type>(n), static_cast<mapped_type>(val));
// 			}


// 			/**
// 			 * @brief Fill Constructor actual function
// 			*/
// 			template <class integer>
// 			void
// 			map_constructor_dispatch (integer n, integer const & val,
// 				allocator_type const &, std::__true_type)	{

// 				if (DEBUG_MODE >= 1)	{
// 					std::cout << "dispatch --> __true_type " << __func__ << std::endl;
// 					std::cout << "CONSTRUCTOR --> fill " << __func__ << std::endl;
// 				}
// 				this->initStorage(n);
// 				initFillmap(n, val);
// 			}

// 			/**
// 			 * @brief Range Constructor actual function
// 			*/
// 			template <class InputIterator>
// 			void
// 			map_constructor_dispatch (InputIterator first, InputIterator last,
// 				 allocator_type const &, std::__false_type)	{

// 				if (DEBUG_MODE >= 1) std::cout << "CONSTRUCTOR --> range : " << __func__ << std::endl;

// 				size_t	n = std::distance(first, last);
// 				this->initStorage(n * 2);
// 				fillmap(first, last);
// 			}


// 			template<typename integer>
// 			void
// 			insert_dispatch(iterator position, integer n, integer val,
// 			std::__true_type)	{
// 				insert(position, static_cast<size_type>(n),
// 					static_cast<mapped_type>(val));
// 			}

// 			template<typename InputIterator>
// 			void
// 			insert_dispatch(iterator position, InputIterator first,
// 			InputIterator last, std::__false_type)	{

// 				if (capacity() == 0)	{
// 					this->initStorage(1);
// 					position = begin();
// 				}

// 				difference_type indexPos = position - begin();
// 				size_type		n = last - first;
// 				if (_size() + n >= capacity())	{
// 					doReserve(capacity() + n + (capacity()>>1));
// 				}

// 				if (begin() + indexPos != end())	{
// 					if (_size() > 1)	{
// 						memMoveRight(begin() + indexPos, end(), n);
// 					}
// 					destroyObjects(this->_head + indexPos, _size() - indexPos);
// 					this->tail += n;
// 					constructObjects(this->_head + indexPos, first, last);
// 				}
// 				else {
// 					constructObjects(this->_head + indexPos, first, last);
// 					this->tail += n;
// 				}
// 			}

// 			/**
// 			 * @brief Construct objects at alocated memory, to be used by
// 			 * constructors
// 			*/
// 			void
// 			fillmap(iterator first, iterator last)	{

// 				for (;first != last; first++)	{
// 					this->_alloc.construct(this->tail, *first);
// 					this->tail++;
// 					if (this->tail == this->tailStorage)
// 						std::cout << "RESIZE HERE" << std::endl;			// not fixed yet
// 				}
// 			}

// 			/**
// 			 * @brief Construct objects at alocated memory, to be used by
// 			 * constructors
// 			*/
// 			void
// 			initFillmap(size_type n, mapped_type const & val)	{

// 				this->tail = this->_head + n;
// 				for (size_t i = 0; i < n; i++){
// 					this->_alloc.construct(this->_head + i, val);
// 				}
// 			}



// 			/**
// 			 * @brief doReserve: No throw version of Reserve.
// 			*/
// 			void
// 			doReserve (size_type n) {

// 				if (n > this->capacity())	{
// 					pointer		oldHead = this->_head;
// 					size_type	oldSize = this->_size();
// 					size_type	oldCapacity = this->capacity();

// 					reallocateBigger(n);

// 					destroyObjects(oldHead, oldSize);
// 					this->_alloc.deallocate(oldHead, oldCapacity);
// 				}
// 			}

// 			void
// 			reallocateBigger(size_type n)	{

// 				if (n > this->capacity())	{

// 					iterator	oldHeadIt = begin();
// 					iterator	oldTailIt = end();

// 					this->initStorage(n);
// 					constructObjects(this->_head, oldHeadIt, oldTailIt);
// 					this->tail += oldTailIt - oldHeadIt;
// 				}
// 			}

// 			void
// 			constructObjects(pointer p, size_t n, mapped_type val = mapped_type())	{
// 				for (size_t i = 0; i < n; i++)	{
// 					this->_alloc.construct(p + i, val);
// 				}
// 			}

// 			void
// 			constructObjects(pointer p, iterator first, iterator last)	{
// 				for (size_t i = 0; first != last; i++, first++)	{
// 					this->_alloc.construct(p + i, *first);
// 				}
// 			}

			// void
			// destroyObjects(pointer p, size_t n)	{
				// for (size_t i = 0; i < n; i++)	{
					// this->_alloc.destroy(p + i);
				// }
			// }

			void
			clearObject( void )	{

				if (DEBUG_MODE >= 3) std::cout << __func__ << std::endl;
				freeAllNodes(_head);
			}

			void
			freeAllNodes( map_node* node )	{

				if (node == NULL)
					return;
				freeAllNodes(node->left);
				freeAllNodes(node->right);
				_allocPair.destroy(node->item);
				_allocPair.deallocate(node->item, 1);
				_allocNode.destroy(node);
				_allocNode.deallocate(node, 1);
			}

// 			/**
// 			 * @brief Move [first, last] range by n memory blocks to theLeft
// 			*/
// 			void
// 			memMoveLeft(iterator first, iterator last, size_t n)	{			// to be tested
// 				while (first != last)	{
// 					constructObjects(first._ptr - n, 1, *first);
// 					destroyObjects(first._ptr, 1);
// 					first++;
// 				}
// 			}


// 			/**
// 			 * @brief Move [first, last] range by n memory blocks to the right
// 			*/
// 			void
// 			memMoveRight(iterator first, iterator last, size_t n)	{
// 				while (last != first)	{
// 					last--;
// 					constructObjects(last._ptr + n, 1, *last);
// 					destroyObjects(last._ptr, 1);
// 				}
// 			}

// 			void
// 			checkRange(size_type n)	const {
// 				if (n > _size())	{
// 					throw std::out_of_range("Out of map's range");
// 				}
// 			}

            value_compare value_comp() const    { return value_compare(_comp); }

		}; // -------------------------------------------------------- Class map



/******************************************************************************.
.******************************************************************************.
.*********** PUBLIC NON MEMBER FUNCTIONS     **********************************.
.******************************************************************************.
.******************************************************************************/






		// template <class T, class Alloc >
		// void
		// swap (map<T,Alloc>& x, map<T,Alloc>& y)	{

		// 	x.swap(y);
		// };

		// template <class T, class Alloc>
		// bool
		// operator== (const map<T,Alloc>& lhs, const map<T,Alloc>& rhs)	{

		// 	if (lhs._size() != rhs._size())
		// 		return false;
		// 	if (lhs.front() != rhs.front() || lhs.back() != rhs.back())
		// 		return false;
		// 	return (std::equal(lhs.begin(), lhs.end(), rhs.begin()));
		// };

		// template <class T, class Alloc>
		// bool
		// operator!= (const map<T,Alloc>& lhs, const map<T,Alloc>& rhs)	{ return(!(lhs == rhs)); };


		// template <class T, class Alloc>
		// bool
		// operator<  (const map<T,Alloc>& lhs, const map<T,Alloc>& rhs)	{

		// 	return (std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));

		// };

		// template <class T, class Alloc>
		// bool
		// operator<= (const map<T,Alloc>& lhs, const map<T,Alloc>& rhs)	{ return(!(lhs > rhs)); };

		// template <class T, class Alloc>
		// bool
		// operator>  (const map<T,Alloc>& lhs, const map<T,Alloc>& rhs)	{

		// 	return (std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));

		// };

		// template <class T, class Alloc>
		// bool
		// operator>= (const map<T,Alloc>& lhs, const map<T,Alloc>& rhs)	{ return(!(lhs < rhs)); };



} // -------------------------------------------------------------- ft namespace


#endif /* *****BVALETTE****** MAP_HPP */
