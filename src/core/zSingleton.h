/**
 * \file
 * \version  $Id: zSingleton.h 62 2013-04-22 13:00:09Z chengxie $
 * \author  , @ztgame.com 
 * \date 2005年12月01日 21时24分24秒 CST
 * \brief 
 *
 * 
 */

#ifndef _SINGLETON_H
#define _SINGLETON_H

#include "Fir.h"
#include "zNoncopyable.h"

namespace Fir {

		template <typename T>
		class Singleton
		{
			private:
				// 禁用拷贝构造函数
				Singleton(const Singleton&);

				// 禁用赋值操作
				const Singleton & operator= (const Singleton &);
			protected:

				Singleton( void )
				{
				}

				~Singleton( void )
				{
				}

			public:

				static T& getMe(void)
				{
					static T s_singleton;
					return s_singleton;
				}

		};
}

/**
 * \brief 使所有的子类在一个进程中唯一且只有一个实例
 *
 */
template <typename T>
class zSingleton : zNoncopyable
{

	protected:

		/**
		 * \brief 缺省构造函数
		 *
		 */
		zSingleton() {};

		/**
		 * \brief 缺省析构函数
		 *
		 */
		virtual ~zSingleton() {};

	public:

		/**
		 * \brief 获取实例
		 *
		 */
		static T& getInstance()
		{
			if (NULL == instace)
				instace = new T();

			return *instace;
		}

		/**
		 * \brief 销毁实例
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instace);
		}

	private:

		/**
		 * \brief 对象实例
		 *
		 */
		static T* instace;
};

template <typename T> T* zSingleton<T>::instace = NULL;

#endif

