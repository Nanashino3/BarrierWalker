#pragma once
#include <functional>

namespace tnl {

	template <class T>
	class Sequence final {
	private:
		T* object_;
		std::function<bool(T*, const float)> now_;
		std::function<bool(T*, const float)> next_;
		bool (T::*p_now_)(const float);
		bool (T::*p_next_)(const float);
		bool is_start_ = true;
		bool is_change_ = false;
		float sum_time_ = 0;
		Sequence() {}
	public:

		//===================================================================================
		// コンストラクタ
		// arg1... 包含クラスの this ポインタを指定
		// arg2... コールバックで呼び出したい包含クラスのメンバメソッドまたはラムダ式指定
		//===================================================================================
		Sequence(T* obj, bool (T::*func)(const float))
			: object_(obj)
			, next_(func)
			, now_(func)
			, p_now_(func)
			, p_next_(func)
		{}

		~Sequence() {}

		//===================================================================================
		// シーケンスのアップデート ( 毎フレーム呼び出せばOK )
		// arg1... フレーム間の経過時間( 秒のデルタタイム )
		//===================================================================================
		inline bool update(const float deltatime) {
			sum_time_ += deltatime;
			bool ret = now_(object_, deltatime);
			if (!is_change_) {
				is_start_ = false;
				return ret;
			}
			now_ = next_;
			p_now_ = p_next_;
			is_start_ = true;
			sum_time_ = 0;
			is_change_ = false;
			return ret;
		}

		//===================================================================================
		// 比較
		//===================================================================================
		inline bool isComparable(bool (T::*func)(const float)) const { return p_now_ == func; }

		//===================================================================================
		// 初期化用　シーケンスの最初の１フレームだけ true が帰る
		//===================================================================================
		inline bool isStart() const { return is_start_; }

		//===================================================================================
		// シーケンスの経過時間を取得 ( 秒 )
		//===================================================================================
		inline float getProgressTime() const { return sum_time_; }

		//===================================================================================
		// シーケンスの変更
		// arg1... 次のフレームから実行させる包含クラスのメソッドを指定
		//===================================================================================
		inline void change(bool (T::*func)(const float)) {
			next_ = func;
			p_next_ = func;
			is_change_ = true;
		}

		//===================================================================================
		// シーケンスを即座に変更
		// arg1... 実行させる包含クラスのメソッドを指定
		// tisp... 次フレームを待たず即座にシーケンスを変更する
		//===================================================================================
		inline void immediatelyChange(bool (T::*func)(const float)) {
			now_ = func;
			p_now_ = func;
			is_start_ = true;
			sum_time_ = 0;
			is_change_ = false;
		}

	};

}

