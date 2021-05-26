#ifndef MAP_HPP
# define MAP_HPP

# include "../utils/ft_pair.hpp"
# include "map_iterator.hpp"

# include <iostream>
# include <cstddef>

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
			typedef typename Allocator::template rebind<map_node>::other	_node_allocator_type;

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


			typedef typename ft::map_iterator<Key, T, Compare, map_node, false>	iterator;
			typedef typename ft::map_iterator<Key, T, Compare, map_node, true>	const_iterator;

            typedef typename ft::reverse_iterator<map_iterator<Key, T, Compare, map_node, false> > reverse_iterator;
            typedef typename ft::reverse_iterator<map_iterator<Key, T, Compare, map_node, true> >  const_reverse_iterator;

/******************************************************************************.
.******************************************************************************.
.*********** NESTED CLASSES          ******************************************.
.******************************************************************************.
.******************************************************************************/

		private:

			struct map_node	{

				map_node( value_type itemSrc ) :	left(NULL),
													parent(NULL),
													right(NULL),
													item(itemSrc) {}
				~map_node( void ) {}

				map_node*		left;
				map_node*		parent;
				map_node*		right;
				value_type		item;
				bool			color;
			};

		public:

			class value_compare	{

				friend class map<Key, T, Compare, Allocator>;
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
			map( const Compare& comp = key_compare(),
			const allocator_type & userAlloc = allocator_type() )	: 	_head(NULL),
																		_dumbNode(NULL),
																		_size(0),
																		_allocNode(userAlloc),
																		_comp(comp)				{
				if (DEBUG_MODE == 1)
					std::cout << "CONSTRUCTOR --> DEFAULT explicit " << __func__ << std::endl;
			}


			/**
			 * @brief Range Constructor
			*/
			template <class InputIterator>
			map (InputIterator first, InputIterator last,
				const key_compare& comp = key_compare(),
				const allocator_type& userAlloc = allocator_type() ) :	_head(NULL),
																		_dumbNode(NULL),
																		_size(0),
																		_allocNode(userAlloc),
																		_comp(comp)				{

				if (DEBUG_MODE == 1)
					std::cout << "CONSTRUCTOR --> Range ! " << __func__ << std::endl;
				insert(first, last);
			}

			/**
			 * @brief Copy Constructor
			*/
			map( map const & src ) :	_head(NULL),
												_dumbNode(NULL),
												_size(0),
												_allocNode(src._allocNode),
												_comp(src._comp)				{

				if (DEBUG_MODE == 1)
					std::cout << "CONSTRUCTOR --> copy " << __func__ << std::endl;

				insert(src.begin(), src.end());
			}


			~map( void )	{

				if (DEBUG_MODE == 1) std::cout << "DESTRUCTOR --> " << __func__ << std::endl;
				clear();
			}

/******************************************************************************.
.******************************************************************************.
.*********** PRIVATE VARIABLES  ***********************************************.
.******************************************************************************.
.******************************************************************************/

			protected:
				map_node*				_head;
				map_node*				_dumbNode;
				size_t					_size;
				_node_allocator_type 	_allocNode;
				Compare	const			_comp;

/******************************************************************************.
.******************************************************************************.
.*********** 🚧  DEBUG 🚧             ******************************************.
.******************************************************************************.
.******************************************************************************/

			private:
			// public:
				void
				debugPrintTree()	{

					if (DEBUG_MODE < 2)
						return ;
					std::cout << "***********************************" << std::endl;
					std::cout << __func__ << "_head is pointing to:  " << _head << std::endl;
					std::cout << "_dumbNode is... : " << std::endl;
					debugPrintNode(_dumbNode);
					std::cout << "Printing tree of size  " << _size << std::endl;

					std::cout << "****************ITERATORES*******************" << std::endl;
					iterator it = begin();
					iterator ite = end();

					for(; it != ite; ++it)	{
						debugPrintNode(it.getPtr());
					}
					std::cout << "***********************************" << std::endl;
				}

				static void
				debugPrintNode( map_node* node )	{

					if (DEBUG_MODE < 2)
						return ;
					if (node != NULL)	{
						std::cout << std::endl;
						std::cout << __func__ << std::endl;
						std::cout << "NODE   " << node << std::endl;
						std::cout << "--- KEY    " << node->item.first << std::endl;
						std::cout << "--  VAL    " << node->item.second << std::endl;
						std::cout << "parent " << node->parent;
						if (node->parent != NULL)
							std::cout << "(" << node->parent->item.first << ";" << node->parent->item.second << ")" << std::endl;
						else
							std::cout << "--> THIS NODE IS _HEAD" << std::endl;

						std::cout << " left   " << node->left;
						if (node->left != NULL)
							std::cout << "(" << node->left->item.first << ";" << node->left->item.second << ")";
						std::cout << std::endl;
						std::cout << " right  " << node->right;
						if (node->right != NULL)
							std::cout << "(" << node->right->item.first << ";" << node->right->item.second << ")";
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

			bool
			empty( void ) const		{ return (_size == 0); }

			size_type
			size( void ) const 		{ return (_size); }

			size_type
			max_size( void ) const	{ return this->_allocNode.max_size(); }

			iterator
			begin( void ) 			{
				if (_dumbNode != NULL)
					return (iterator(_dumbNode->left, _dumbNode, _comp));
				return (iterator());
			}

			const_iterator
			begin( void ) const		{
				if (_dumbNode != NULL)
					return (const_iterator(_dumbNode->left, _dumbNode, _comp));
				return (const_iterator());
			}

			iterator
			end( void ) 	 		{
				if (_dumbNode != NULL)
					return (iterator(_dumbNode, _dumbNode, _comp));
				return (iterator());
			}

			const_iterator
			end( void ) const 		{
				if (_dumbNode != NULL)
					return (const_iterator(_dumbNode, _dumbNode, _comp));
				return (const_iterator());
			}

			reverse_iterator
			rbegin( void ) 			{	return reverse_iterator(--end());	}

			const_reverse_iterator
			rbegin( void ) 	const	{	return const_reverse_iterator(--end());	}

			reverse_iterator
			rend( void ) 			{	return reverse_iterator(end());	}

			const_reverse_iterator
			rend( void ) 	const	{	return const_reverse_iterator(end());	}

			iterator
			find (const key_type& k)	{

				if (empty() == true)
					return(iterator());
				map_node*	nodeFound = locateNode(_head, k);
				if (nodeFound == NULL)
					return (end());
				return (iterator(nodeFound, _dumbNode, _comp));
			}

			const_iterator
			find (const key_type& k) const	{

				if (empty() == true)
					return(const_iterator());
				map_node* const	nodeFound = locateNode(_head, k);
				if (nodeFound == NULL)
					return (end());
				return (const_iterator(nodeFound, _dumbNode, _comp));
			}

			size_type
			count (const key_type& k) const	{
				return (find(k) != end() ? 1 : 0);
			}

			iterator
			lower_bound (const key_type& k) {

				if (empty() == true)
					return(iterator());

				map_node* const	nodeFound = locateBound(_head, k, isLowerBoundNode);

				if (nodeFound == NULL)
					return (end());
				else
					return (iterator(nodeFound, _dumbNode, _comp));
			}

			const_iterator
			lower_bound (const key_type& k) const	{

				if (empty() == true)
					return(const_iterator());

				map_node* const	nodeFound = locateBound(_head, k, isLowerBoundNode);

				if (nodeFound == NULL)
					return (end());
				else
					return (const_iterator(nodeFound, _dumbNode, _comp));
			}


			iterator
			upper_bound (const key_type& k) {

				if (empty() == true)
					return(iterator());

				map_node* const	nodeFound = locateBound(_head, k, isUpperBoundNode);

				if (nodeFound == NULL)
					return (end());
				else
					return (iterator(nodeFound, _dumbNode, _comp));
			}

			const_iterator
			upper_bound (const key_type& k) const	{

				if (empty() == true)
					return(const_iterator());

				map_node* const	nodeFound = locateBound(_head, k, isUpperBoundNode);

				if (nodeFound == NULL)
					return (end());
				else
					return (const_iterator(nodeFound, _dumbNode, _comp));
			}


			pair<iterator,iterator>
			equal_range (const key_type& k)	{

				return (ft::make_pair(lower_bound(k), upper_bound(k)));
			}

			pair<const_iterator, const_iterator>
			equal_range (const key_type& k) const	{

				return (ft::make_pair(lower_bound(k), upper_bound(k)));
			}

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

				map_node*	posPtr = position.getPtr();
				if (posPtr == NULL)
					return (iterator());
				if (position != end() && lower_bound(val.first) == position)	{
					if (DEBUG_MODE >= 2) std::cout << __func__ << ":	 Insert with optimization." << std::endl;
					return (btree_insert_data(posPtr->parent, &posPtr, val).first);
				}
				else
					return (btree_insert_data(NULL, &_head, val).first);
			}

			/**
			 * @brief Range insert from first to last element
			*/
			template <class InputIterator>
 			void
			insert (InputIterator first, InputIterator last)	{

				for (; first != last; first++)
					insert(*first);
			}

			void
			erase( iterator position )	{

				map_node*	deadNode = position.getPtr();
				map_node*	deadNodeLeft = deadNode->left;
				map_node*	deadNodeRight = deadNode->right;
				map_node*	singleChild = getSingleChild(deadNode);

				if (deadNode == NULL)
					return;
				if (isLeaf(deadNode) == true)
					detachFromParent(deadNode);
				else if (singleChild != NULL)
					detachFromParent(deadNode, singleChild);
				else	{
					detachFromParent(deadNode, deadNodeLeft);
					map_node*	farRight = getFarRight(deadNodeLeft);
					deadNodeRight->parent = farRight;
					farRight->right = deadNodeRight;
				}
				decSize();
				btree_update_dumbNode();
				freeNode(deadNode);
			}

			size_type
			erase( const key_type& k )	{

				map_node*	target = btree_search_key(_head, k);
				if (target == NULL)
					return 0;
				else	{
					erase(iterator(target, _dumbNode, _comp));
					return (1);
				}
			}

			void
			erase (iterator first, iterator last)	{

				iterator next;
				while (first != last)	{
					next = first;
					next++;
					erase(first);
					first = next;
				}
			}

			void
			swap (map& src)	{

				if (this->_head != src._head)	{
					map_node*				tmp_head = src._head;
					map_node*				tmp_dumbNode = src._dumbNode;
					size_t					tmp_size = src._size;
					_node_allocator_type 	tmp_allocNode = src._allocNode;

					src._head = this->_head;
					src._dumbNode = this->_dumbNode;
					src._size = this->_size;
					src._allocNode = this->_allocNode;

					this->_head = tmp_head;
					this->_dumbNode = tmp_dumbNode;
					this->_size = tmp_size;
					this->_allocNode = tmp_allocNode;
				}
			}

			map&
			operator= (const map& src)	{

				if (this->_head != src._head)	{
					clear();

					if (src.empty() == false)	{
						if (src.size() > 2)	{
							const_iterator	half = src.begin();
							for (size_t i = 0; i < src.size() / 2; i++)
								half++;
							insert(*half);
						}
						insert(src.begin(), src.end());
					}
				}
				return *this;
			}

			mapped_type&
			operator[]( const key_type& key )	{

				value_type					insertValue(key, mapped_type());
				ft::pair<iterator, bool>	ret = insert(insertValue);
				return (ret.first->second);
			}

			void
			clear( void )			{

				if (DEBUG_MODE >= 3) std::cout << __func__ << std::endl;
				freeAllNodes(_head);
				freeNode(_dumbNode);
				_size = 0;
				_head = NULL;
				_dumbNode = NULL;
			}

            value_compare	value_comp( void ) const	{ return value_compare(_comp); }
            key_compare		key_comp( void ) const		{ return key_compare(_comp); }

/******************************************************************************.
.******************************************************************************.
.*********** PRIVATE MEMBER FUNCTIONS AND HELPERS  ****************************.
.******************************************************************************.
.******************************************************************************/

 		private:

			void
			detachFromParent( map_node* node, map_node* newChild = NULL )	{
				map_node* parent = node->parent;
				if (parent != NULL)	{
					if (parent->left == node)
						parent->left = newChild;
					else if (parent->right == node)
						parent->right = newChild;
				}
				else if (node == _head)
					_head = newChild;
				if (newChild != NULL)
					newChild->parent = parent;
				node->parent = NULL;
			}

			static map_node*
			getSingleChild( map_node* node )	{

				if (node->right != NULL && node->left == NULL)
					return (node->right);
				else if (node->right == NULL && node->left != NULL)
					return (node->left);
				else
					return (NULL);
			}

			map_node*
			locateBound( map_node* root, const key_type& key, bool (*isBound)(map_node*, const key_type&) ) const	{

				if (root == _head && isBound(_dumbNode->left, key) == true)
					return (_dumbNode->left);
				else if (root == _head && isBound(_dumbNode->right, key) == false)
					return (NULL);

				map_node* candidate = root;
				map_node* bestCandidate = NULL;
				while (candidate != NULL)	{
					if (isBound(candidate, key) == true)	{
						bestCandidate = candidate;
						candidate = candidate->left;
					}
					else
						candidate = candidate->right;
				}
				return (bestCandidate);
			}

			map_node*
			locateNode( map_node* root, const key_type& key ) const	{

				if (root != NULL)	{
					if (_comp(key, root->item.first) == true)
						return (locateNode(root->left, key));
					else if (isEqualKey(key, root->item.first) == false)
						return (locateNode(root->right, key));
					else
						return (root);
				}
				else
					return (NULL);
			}

			static bool
			isLowerBoundNode( map_node* node, const key_type& key ) {

			typename ft::map<Key, T, Compare> tmpObj;
			typename ft::map<Key, T, Compare>::key_compare cmpFunc = tmpObj.key_comp();

				return (node != NULL
					&& (cmpFunc(node->item.first, key) == false
						|| isEqualKey(node->item.first, key) == true));
			}

			static bool
			isUpperBoundNode( map_node* node, const key_type& key ) {
				typename ft::map<Key, T, Compare> tmpObj;
				typename ft::map<Key, T, Compare>::key_compare cmpFunc = tmpObj.key_comp();
				return (node != NULL && cmpFunc(key, node->item.first) == true);
			}

			void
			btree_update_dumbNode( map_node* node )	{
				if (_dumbNode == NULL)
					btree_init_dumbNode();
				if (node == getFarLeft(_head))
					_dumbNode->left = node;
				if (node == getFarRight(_head))
					_dumbNode->right = node;
			}

			void
			btree_update_dumbNode( void )	{
				if (_dumbNode == NULL)
					btree_init_dumbNode();
				_dumbNode->left = getFarLeft(_head);
				_dumbNode->right = getFarRight(_head);
			}

			void
			btree_init_dumbNode( void )	{

				if (_dumbNode == NULL)	{
					_dumbNode = _allocNode.allocate(1);
					_allocNode.construct(_dumbNode, map_node(value_type()));
				}
			}

			map_node*
			btree_create_node(map_node* parent, key_type k, mapped_type val)	{

				map_node*	newNode = _allocNode.allocate(1);
				_allocNode.construct(newNode, map_node(value_type(k ,val)));
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
					if (_comp(pairSrc.first, tree->item.first) == true)
						return (btree_insert_data(tree, &tree->left, pairSrc));
					else if (isEqualKey(pairSrc.first, tree->item.first) == false)
						return (btree_insert_data(tree, &tree->right, pairSrc));
					else
						return (ft::pair<iterator, bool>(iterator(*root, _dumbNode, _comp), false));
				}
				else	{
					*root = btree_create_node(parent, pairSrc.first, pairSrc.second);
					btree_update_dumbNode(*root);
					incSize();
					return (ft::pair<iterator, bool>(iterator(*root, _dumbNode, _comp), true));
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


			map_node*
			btree_search_key(map_node* root, const key_type& targetKey)	{

				if (root != NULL)	{
					if (_comp(targetKey, root->item.first) == true)
						return (btree_search_key(root->left, targetKey));
					else if (_comp(root->item.first, targetKey) == true)
						return (btree_search_key(root->right, targetKey));
				}
				return (root);
			}

			static map_node*
			getFarLeft( map_node* cursor )  {

				while (cursor != NULL && cursor->left != NULL)
					cursor = cursor->left;
				return (cursor);
			}

			static map_node*
			getFarRight( map_node* cursor )  {

				while (cursor != NULL && cursor->right != NULL)
					cursor = cursor->right;
				return (cursor);
			}

			static bool
			isLeaf(map_node* node)  {
				return (node->left == NULL && node->right == NULL);
			}

			static bool
			isEqualKey(const Key& existingKey, const Key& newKey) {
				typename ft::map<Key, T, Compare> tmpObj;
				typename ft::map<Key, T, Compare>::key_compare cmpFunc = tmpObj.key_comp();
				return (cmpFunc(existingKey, newKey) == false
				&& cmpFunc(newKey, existingKey) == false);
			}


			size_t
			incSize( size_t inc = 1 ) { _size += inc; return(_size); }

			size_t
			decSize( size_t inc = 1 ) { _size -= inc; return(_size); }

			void
			freeNode( map_node* node)	{
				if (node != NULL)	{
					_allocNode.destroy(node);
					_allocNode.deallocate(node, 1);
				}
			}

			void
			freeAllNodes( map_node* root )	{

				if (root == NULL)
					return;
				freeAllNodes(root->left);
				freeAllNodes(root->right);
				freeNode(root);
			}

			allocator_type
			get_allocator() const	{
				return _allocNode();
			}

		}; // -------------------------------------------------------- Class map

/******************************************************************************.
.******************************************************************************.
.*********** PUBLIC NON MEMBER FUNCTIONS     **********************************.
.******************************************************************************.
.******************************************************************************/

		template <class Key, class T, class Compare, class Alloc>
		void
		swap (map<Key,T,Compare,Alloc>& x, map<Key,T,Compare,Alloc>& y)	{

			x.swap(y);
		};

} // -------------------------------------------------------------- ft namespace

#endif /* *****BVALETTE****** MAP_HPP */
