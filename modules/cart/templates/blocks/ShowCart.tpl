{*
 * $Revision: 15342 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{g->callback type="cart.LoadCart"}
<div class="{$class}">
  <h3> {g->text text="Your Cart"} </h3>
  <p>
    {g->text one="You have %d item in your cart" many="You have %d items in your cart"
	     count=$block.cart.ShowCart.total arg1=$block.cart.ShowCart.total}
  </p>
  <a class="{g->linkId view="cart.ViewCart"}" href="{g->url arg1="view=cart.ViewCart" arg2="return=true"}">{g->text text="View Cart"}</a>
</div>
