import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from datetime import datetime

from helpers import apology, login_required, lookup, usd

# configure application
app = Flask(__name__)

# ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # forget any user_id
    session.clear()

    # user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username")
        pass1 = request.form.get("password")
        pass2 = request.form.get("confirmation")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=username)

        # ensure username is distinct
        if len(rows) != 0:
            # return apology("Username Already Taken", 403)
            return "<script>alert('Username Already Exists')</script>"

        # ensuring the password is typed in correctly twice
        elif pass1 != pass2:
            return apology("passwords don't match!", 400)

        else:
            # generating Hash of password
            password = generate_password_hash(pass1)

            # inserting new user and hash
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)", username=username, password=password)

            # remember which user has logged in
            session["user_id"] = db.execute("SELECT id FROM users WHERE username = :username", username=username)[0]["id"]

            # redirect user to home page
            return redirect("/")

    # user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # forget any user_id
    session.clear()

    # user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect("/")

    # user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # getting number of shares
    stocks = db.execute("SELECT stock_symbol, no_shares FROM stocks WHERE user_id = ?", session["user_id"])

    # getting user's cash
    total = cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # creating a dictionary list with name, symbol, price, shares and total
    stock_info = []

    for i in range(len(stocks)):
        # looking up for the price and name for a symbol
        stock_info.append(lookup(stocks[i]["stock_symbol"]))
    # storing number of shares
        stock_info[i]["shares"] = stocks[i]["no_shares"]
    # storing Total value of shares as USD
        stock_info[i]["total"] = usd(stock_info[i]["shares"] * stock_info[i]["price"])
    # calculating total of all the share values
        total += stock_info[i]["shares"] * stock_info[i]["price"]
    # converting price in USD
        stock_info[i]["price"] = usd(stock_info[i]["price"])

    return render_template("index.html", stocks=stock_info, cash=usd(cash), total=usd(total))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # user reached route via POST
    if request.method == "POST":

        # get stock information
        stock = lookup(request.form.get("symbol"))

        # check for stock symbol
        if stock == None:
            return apology("Invalid Symbol", 400)

        else:
            return render_template("quoted.html", stock=stock)

    # user reached via GET
    else:
        return render_template("quote.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        stock = lookup(request.form.get("symbol"))

        if stock == None:
            return apology("Invalid Symbol")

        # getting the number of shares
        shares = int(request.form.get("shares"))
        # getting cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        # ensures user has enough cash
        req_amount = shares * stock["price"]
        if req_amount > cash:
            return apology("Insufficient balance to make this purchase")

        prev_stocks = db.execute("SELECT no_shares FROM stocks WHERE user_id = ? AND stock_symbol = ?",
                                session["user_id"], stock["symbol"])

        # checks if user owns the mentioned stock
        if len(prev_stocks) != 0:
            prev_shares = prev_stocks[0]["no_shares"]
            # update current number of shares
            db.execute("UPDATE stocks SET no_shares = ? WHERE user_id = ? AND stock_symbol = ?",
                        prev_shares+shares, session["user_id"], stock["symbol"])
            # update cash
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash-req_amount, session["user_id"])
        # if stock isn't already owned, add it to the database
        else:
            db.execute("INSERT INTO stocks (user_id, stock_symbol, no_shares) VALUES (?, ?, ?)",
                        session["user_id"], stock["symbol"], shares)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash-req_amount, session["user_id"])

        # updating history
        now = datetime.now()
        transacted = now.strftime("%H:%M:%S %d/%m/%Y")
        db.execute("INSERT INTO history (id, symbol, price, total, shares, datetime) VALUES (?, ?, ?, ?, ?, ?)",
                    session["user_id"], stock["symbol"], usd(stock["price"]), usd(req_amount), shares, transacted)

        return redirect("/")

    # user reached route via GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    stocks = db.execute("SELECT symbol, shares, price, total, datetime FROM history WHERE id = ? ORDER BY sr DESC", session["user_id"])
    return render_template("history.html", stocks=stocks)


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # user reached route via POST
    if request.method == "POST":

        shares = int(request.form.get("shares"))
        if shares == 0:
            return redirect("/")

        # looks for stock
        stock = lookup(request.form.get("symbol"))

        # information about owned stocks
        prev_stocks = db.execute("SELECT no_shares FROM stocks WHERE user_id = ? AND stock_symbol = ?",
                                session["user_id"], stock["symbol"])

        # existing shares
        prev_shares = prev_stocks[0]["no_shares"]

        if shares > prev_shares:
            return apology("Insufficient number of shares")

        # gets cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # cost of shares
        amount = shares*stock["price"]

        # updating the number of shares
        db.execute("UPDATE stocks SET no_shares = ? WHERE user_id = ? AND stock_symbol = ?",
                   prev_shares-shares, session["user_id"], stock["symbol"])

        # updating cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash+amount, session["user_id"])


        # updating history
        now = datetime.now()
        transacted = now.strftime("%H:%M:%S %d/%m/%Y")
        db.execute("INSERT INTO history (id, symbol, price, total, shares, datetime) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], stock["symbol"], usd(stock["price"]), usd(amount), -shares, transacted)

        return redirect("/")

    # user reached route via GET
    else:
        stocks = db.execute("SELECT stock_symbol, no_shares FROM stocks WHERE user_id = ?", session["user_id"])

        return render_template("sell.html", stocks=stocks)

@app.route("/settings", methods=["GET", "POST"])
def passwordchange():
    """Change Password"""
    if request.method == "POST":

        old = request.form.get("old")
        npass1 = request.form.get("pass1")
        npass2 = request.form.get("pass2")

        # Ensuring the password is typed in correctly twice
        if npass1 != npass2:
            return apology("Passwords don't match")

        # Query database for password
        hashed = db.execute("SELECT hash FROM users WHERE id = :id", id=session["user_id"])[0]["hash"]

        if not check_password_hash(hashed, old):
            return apology("Incorrect Password")

        else:
            # Generating Hash of password
            npassword = generate_password_hash(npass1)

            # Updating new hash
            db.execute("UPDATE users SET hash = ? WHERE id = ?", npassword, session["user_id"])

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("settings.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
